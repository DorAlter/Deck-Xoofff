use core::simd::{LaneCount, Simd, SupportedLaneCount};
use crunchy::unroll;

/// Maximum number of rounds one can request to have when applying Xoodoo\[n_r\] permutation i.e. n_r <= MAX_ROUNDS
///
/// See table 2 of https://ia.cr/2018/767
const MAX_ROUNDS: usize = 12;

/// Xoodoo\[n_r\] round constants, taken from table 2 of https://ia.cr/2018/767
const RC: [u32; MAX_ROUNDS] = [
    0x00000058, 0x00000038, 0x000003c0, 0x000000d0, 0x00000120, 0x00000014, 0x00000060, 0x0000002c,
    0x00000380, 0x000000f0, 0x000001a0, 0x00000012,
];

#[inline(always)]
pub fn cyclic_shiftx<const N: usize, const T: usize, const V: u32>(
    plane: &[Simd<u32, N>],
) -> [Simd<u32, N>; 4]
where
    LaneCount<N>: SupportedLaneCount,
{
    debug_assert!(
        plane.len() == 4,
        "Each lane of Xoodoo permutation state must have four lanes !"
    );

    let shl = Simd::<u32, N>::splat(V);
    let shr = Simd::<u32, N>::splat(32 - V);

    let mut shifted = [Simd::<u32, N>::splat(0u32); 4];
    unroll! {
        for i in 0..4 {
            shifted[(T + i) & 3usize] = (plane[i] << shl) | (plane[i] >> shr);
        }
    }

    shifted
}

#[inline(always)]
fn roundx<const N: usize>(state: &mut [Simd<u32, N>], round_key: u32)
where
    LaneCount<N>: SupportedLaneCount, {
    
    let p  = [
        state[0] ^ state[4] ^ state[8],
        state[1] ^ state[5] ^ state[9],
        state[2] ^ state[6] ^ state[10],
        state[3] ^ state[7] ^ state[11],
    ];
        
    let t0 = cyclic_shiftx::<N, 1, 5>(&p);
    let t1 = cyclic_shiftx::<N, 1, 14>(&p);

    let mut e = [Simd::<u32, N>::splat(0u32); 4];
    unroll! {
        for i in 0..4 {
            e[i] = t0[i] ^ t1[i];
        }
    }

    let mut tmp = [Simd::<u32, N>::splat(0u32); 12];

    tmp[0] = e[0] ^ state[0] ^ Simd::<u32, N>::splat(round_key);
    tmp[1] = e[1] ^ state[1];
    tmp[2] = e[2] ^ state[2];
    tmp[3] = e[3] ^ state[3];

    tmp[4] = e[3] ^ state[7];
    tmp[5] = e[0] ^ state[4];
    tmp[6] = e[1] ^ state[5];
    tmp[7] = e[2] ^ state[6];

    let shl = Simd::<u32, N>::splat(11);
    let shr = Simd::<u32, N>::splat(21);

    tmp[8] = e[0] ^ state[8];
    tmp[8] = (tmp[8] <<  shl) | (tmp[8] >>  shr);
    tmp[9] = e[1] ^ state[9];
    tmp[9] = (tmp[9] <<  shl) | (tmp[9] >>  shr);
    tmp[10] = e[2] ^ state[10];
    tmp[10] = (tmp[10] << shl) | (tmp[10] >> shr);
    tmp[11] = e[3] ^ state[11];
    tmp[11] = (tmp[11] << shl) | (tmp[11] >> shr);

    state[0] = (!tmp[4] & tmp[8]) ^ tmp[0];
    state[1] = (!tmp[5] & tmp[9]) ^ tmp[1];
    state[2] = (!tmp[6] & tmp[10]) ^ tmp[2];
    state[3] = (!tmp[7] & tmp[11]) ^ tmp[3];

    let shl = Simd::<u32, N>::splat(1);
    let shr = Simd::<u32, N>::splat(31);

    state[4] = (!tmp[8] & tmp[0]) ^ tmp[4];
    state[4] = (state[4] << shl) | (state[4] >> shr);
    state[5] = (!tmp[9] & tmp[1]) ^ tmp[5];
    state[5] = (state[5] << shl) | (state[5] >> shr);
    state[6] = (!tmp[10] & tmp[2]) ^ tmp[6];
    state[6] = (state[6] << shl) | (state[6] >> shr);
    state[7] = (!tmp[11] & tmp[3]) ^ tmp[7];
    state[7] = (state[7] << shl) | (state[7] >> shr);

    let shl = Simd::<u32, N>::splat(8);
    let shr = Simd::<u32, N>::splat(24);
    state[8] = (!tmp[2] & tmp[6]) ^ tmp[10];
    state[8] = (state[8] << shl) | (state[8] >> shr);
    state[9] = (!tmp[3] & tmp[7]) ^ tmp[11];
    state[9] = (state[9] << shl) | (state[9] >> shr);
    state[10] = (!tmp[0] & tmp[4]) ^ tmp[8];
    state[10] = (state[10] << shl) | (state[10] >> shr);
    state[11] = (!tmp[1] & tmp[5]) ^ tmp[9];
    state[11] = (state[11] << shl) | (state[11] >> shr);
}

/// Xoodoo\[n_r\] permutation function s.t. n_r ( <= MAX_ROUNDS ) times round function
/// is applied on permutation state, as described in algorithm 1 of https://ia.cr/2018/767.
#[inline(always)]
pub fn permutex<const N: usize, const ROUNDS: usize>(state: &mut [Simd<u32, N>])
where
    LaneCount<N>: SupportedLaneCount,
{
    debug_assert!(
        state.len() == 12,
        "Xoodoo permutation state must have 12 lanes !"
    );
    debug_assert!(
        ROUNDS <= MAX_ROUNDS,
        "Requested rounds must be < MAX_ROUNDS !"
    );

    let start = MAX_ROUNDS - ROUNDS;
    for ridx in start..MAX_ROUNDS {
        roundx(state, RC[ridx]);
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_xoodoo_simd() {
        use crate::simd::xoodoo::permutex;
        use crate::xoodoo::permute;
        use core::simd::u32x2;
        use rand::{thread_rng, Rng};

        let mut rng = thread_rng();

        let mut state1 = [0u32; 12];
        let mut state2 = [0u32; 12];

        rng.fill(&mut state1);
        rng.fill(&mut state2);

        let mut statex2 = [u32x2::splat(0u32); 12];
        for i in 0..12 {
            statex2[i] = u32x2::from_slice(&[state1[i], state2[i]]);
        }

        permute::<12>(&mut state1);
        permute::<12>(&mut state2);
        permutex::<2, 12>(&mut statex2);

        let mut state12 = [0u32; 12];
        let mut state22 = [0u32; 12];
        for i in 0..12 {
            let [s1, s2] = statex2[i].to_array();
            state12[i] = s1;
            state22[i] = s2;
        }

        assert_eq!(state1, state12);
        assert_eq!(state2, state22);
    }
}