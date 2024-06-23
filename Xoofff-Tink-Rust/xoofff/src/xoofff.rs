use crate::rolling;
use crate::xoodoo;
use crunchy::unroll;
use std::cmp;

/// Xoodoo\[n_r\] being a 384 -bit permutation, messages are consumed in 48 -bytes chunks
const BLOCK_SIZE: usize = 48;

/// \# -of rounds for Xoodoo permutation, see definition 3 of https://ia.cr/2018/767
const ROUNDS: usize = 6;

/// \# -of lanes ( each of 32 -bit width ) in Xoodoo permutation state
const LANE_CNT: usize = BLOCK_SIZE / std::mem::size_of::<u32>();

/// Xoofff is a deck function, obtained by instantiating Farfalle construction with
/// Xoodoo\[6\] permutation and two rolling functions, having nice incremental input/
/// output processing capability, offering ability of restarting `absorb->squeeze`
/// cycle arbitrary number of times, so that arbitrary number of message sequences ( s.t.
/// each message itself is arbitrary bytes wide ) can be consumed in very flexible fashion.
///
/// One can absorb arbitrary bytes wide message into deck function state and then
/// squeeze any number of bytes. Once done with squeezing, when new message arrives,
/// `absorb->squeeze` cycle can be restarted by calling `restart` function, which
/// will prepare deck function state so that new message ( of arbitrary bytes ) can be consumed
/// into deck function state. This way one can absorb messages from a sequence of arbitrary length.
/// See https://ia.cr/2016/1188 for definition of Farfalle.
/// Also see https://ia.cr/2018/767 for definition of Xoofff.
#[derive(Clone, Copy)]
pub struct Xoofff {
    imask: [u32; LANE_CNT], // input mask
    omask: [u32; LANE_CNT], // output mask
    acc: [u32; LANE_CNT],   // accumulator
    iblk: [u8; BLOCK_SIZE], // input message block ( buffer )
    ioff: usize,            // offset into input message block
    ooff: usize,            // offset into output message block
}

impl Xoofff {
    /// Create a new instance of Xoofff, with a key of byte length < 48, which
    /// can be used for incrementally absorbing messages and squeezing output bytes.
    #[inline(always)]
    pub fn new(key: &[u8]) -> Self {
        debug_assert!(
            key.len() < BLOCK_SIZE,
            "Key byte length must be < {}",
            BLOCK_SIZE
        );

        // masked key derivation phase
        let padded_key = pad10x(key);
        let mut masked_key = bytes_to_le_words(&padded_key);
        xoodoo::permute::<ROUNDS>(&mut masked_key);

        Self {
            imask: masked_key,
            omask: [0u32; LANE_CNT],
            acc: [0u32; LANE_CNT],
            iblk: [0u8; BLOCK_SIZE],
            ioff: 0,
            ooff: 0,
        }
    }

    /// Given a message M of byte length N (>=0), this routine can be used for absorbing
    /// message bytes into the state of the deck function Xoofff, following algorithm 1,
    /// defined in Farfalle specification https://ia.cr/2016/1188.
    #[inline(always)]
    pub fn absorb(&mut self, msg: &[u8], domain_seperator: u8, ds_bit_width: usize) {

        let blk_cnt = msg.len() / BLOCK_SIZE;
        let mut moff = 0;

        for _ in 0..blk_cnt {
            let byte_cnt = BLOCK_SIZE;
            let mut words = bytes_to_le_words(&msg[moff..(moff + byte_cnt)]);

            debug_assert_eq!(LANE_CNT, 12);
            unroll! {
                for i in 0..12 {
                    words[i] ^= self.imask[i];
                }
            }

            xoodoo::permute::<ROUNDS>(&mut words);

            debug_assert_eq!(LANE_CNT, 12);
            unroll! {
                for i in 0..12 {
                    self.acc[i] ^= words[i];
                }
            }

            rolling::roll_xc(&mut self.imask);
            moff += byte_cnt;
        }

        let rm_bytes = msg.len() - moff;

        self.iblk[0..rm_bytes].copy_from_slice(&msg[moff..]);
        self.ioff += rm_bytes;


        let mask = (1u8 << ds_bit_width) - 1u8;
        let pad_byte = (1u8 << ds_bit_width) | (domain_seperator & mask);

        self.iblk[self.ioff..].fill(0);
        self.iblk[self.ioff] = pad_byte;

        let mut words = bytes_to_le_words(&self.iblk);

        debug_assert_eq!(LANE_CNT, 12);
        unroll! {
            for i in 0..12 {
                words[i] ^= self.imask[i];
            }
        }

        xoodoo::permute::<ROUNDS>(&mut words);

        debug_assert_eq!(LANE_CNT, 12);
        unroll! {
            for i in 0..12 {
                self.acc[i] ^= words[i];
            }
        }

        rolling::roll_xc(&mut self.imask);
        rolling::roll_xc(&mut self.imask);

        self.ioff = 0;

        self.omask.copy_from_slice(&self.acc);
        xoodoo::permute::<ROUNDS>(&mut self.omask);

        let mut words = self.omask;
        xoodoo::permute::<ROUNDS>(&mut words);

        debug_assert_eq!(LANE_CNT, 12);
        unroll! {
            for i in 0..12 {
                words[i] ^= self.imask[i];
            }
        }

        words_to_le_bytes(&words, &mut self.iblk);
        self.ooff = 0;

        rolling::roll_xe(&mut self.omask);
    }

    /// Performs xor between buf_out and buf_in and store results in buf_out
    #[inline(always)]
    fn _xor(&self, buf_out: &mut [u8], buf_in: &[u8] ){
        for (plain, s) in buf_out.iter_mut().zip(buf_in.iter()) {
            *plain ^= s;
        }
    }
    
    /// Given that N -many message bytes are already absorbed into deck function state, 
    /// this routine can be used for squeezing arbitrary many bytes out of deck function
    ///  state. One can call this function arbitrary many times, each time requesting 
    /// arbitrary many bytes.
    /// This routine implements last portion of algorithm 1 of https://ia.cr/2016/1188.
    #[inline(always)]
    pub fn squeeze(&mut self, out: &mut [u8]) {

        let mut off = 0;

        while off < out.len() {
            let read = cmp::min(BLOCK_SIZE - self.ooff, out.len() - off);

            self._xor(&mut out[off..(off + read)], &self.iblk[self.ooff..(self.ooff + read)]);

            self.ooff += read;
            off += read;

            if self.ooff == BLOCK_SIZE {
                let mut words = self.omask;
                xoodoo::permute::<ROUNDS>(&mut words);

                debug_assert_eq!(LANE_CNT, 12);
                unroll! {
                    for i in 0..12 {
                        words[i] ^= self.imask[i];
                    }
                }

                words_to_le_bytes(&words, &mut self.iblk);
                self.ooff = 0;

                rolling::roll_xe(&mut self.omask);
            }
        }
    }

    /// Given that a message of arbitrary byte length is absorbed into deck function state,
    /// this function can be invoked when you've new message waiting to be absorbed into 
    /// deck function state and you need to restart the `absorb->squeeze` cycle.
    /// This routine implements portion of algorithm 1 of https://ia.cr/2016/1188.
    #[inline(always)]
    pub fn restart(&mut self) {

        self.omask.fill(0);
        self.iblk.fill(0);
        self.ooff = 0;
    }
}

/// Given a message of length N -bytes ( s.t. N < 48 ), this routine pads the
/// message following pad10* rule such that padded message length becomes 48 -bytes.
#[inline(always)]
pub(crate) fn pad10x(msg: &[u8]) -> [u8; BLOCK_SIZE] {
    debug_assert!(
        msg.len() < BLOCK_SIZE,
        "Paddable message length must be < {}",
        BLOCK_SIZE
    );

    let mlen = msg.len();
    let mut res = [0u8; BLOCK_SIZE];

    res[..mlen].copy_from_slice(msg);
    res[mlen] = 0x01;

    res
}

/// Given a byte array of length 48, this routine interprets those bytes as 12 unsigned
/// 32 -bit integers (= u32) s.t. four consecutive bytes are placed in little endian order
/// in a u32 word.
#[inline(always)]
pub(crate) fn bytes_to_le_words(bytes: &[u8]) -> [u32; LANE_CNT] {
    let mut words = [0u32; LANE_CNT];

    debug_assert_eq!(LANE_CNT, 12);
    unroll! {
        for i in 0..12 {
            words[i] = u32::from_le_bytes(bytes[i * 4..(i + 1) * 4].try_into().unwrap());
        }
    }
    words
}

#[inline(always)]
pub(crate) fn words_to_le_bytes(words: &[u32; LANE_CNT], bytes: &mut [u8; BLOCK_SIZE]) {
    debug_assert_eq!(LANE_CNT, 12);
    unroll! {
        for i in 0..12 {
            bytes[i * 4..(i + 1) * 4].copy_from_slice(&words[i].to_le_bytes());
        }
    }
}
