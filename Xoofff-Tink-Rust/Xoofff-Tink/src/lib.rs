use xoofff::Xoofff;

#[cfg(test)]
mod tests;

/// The length of the authentication tags (in bytes).
const TAG_LEN: usize = 32;

/// Length of the domain seperation (in bits).
const DS_BIT_LEN: usize = 1;

/// The length of the counter (in bytes).
const COUNTER_LEN: usize = 8;

/// The length of the counter plus the authentication tags (in bytes).
const COUNTER_TAG_LEN: usize = TAG_LEN + COUNTER_LEN;

pub struct Deck {
    xoofff: Xoofff,
}

#[derive(Debug)]
pub enum Error {
    WrongTag,
}

impl Deck {
    pub fn new(key: &[u8], nonce: &[u8]) -> Self {
        let mut xoofff = Xoofff::new(key);

        xoofff.absorb(nonce, 0, 0);
        xoofff.restart();

        Deck { xoofff}
    }

    #[inline(always)]
    fn _absorb_squeeze(&mut self, deck: &mut Xoofff, msg: &[u8], domain_seperator: u8, out : &mut [u8] ){
        deck.absorb(msg, domain_seperator, DS_BIT_LEN);
        deck.squeeze(out);
        deck.restart();
    }

    #[inline(always)]
    fn _wrap(&mut self, plain: &mut Vec<u8>, counter: &[u8; COUNTER_LEN]) -> Result<(), Error> {
        let mut cloned = self.xoofff.clone();
        let mut tag = [0u8; TAG_LEN];

        if plain.len() > 0 {
            self._absorb_squeeze(&mut cloned, counter, 0b0, plain);
            self._absorb_squeeze(&mut cloned, &plain.as_slice(), 0b1, &mut tag);

        } else {
            self._absorb_squeeze(&mut cloned, counter, 0b1, &mut tag);
        };
        
        plain.extend_from_slice(counter);
        plain.extend_from_slice(&tag);

        Ok(())
    }

    #[inline(always)]
    fn _wrap_inplace_detached(&mut self, plain: &mut Vec<u8>, tag: &mut [u8; TAG_LEN], counter: &[u8; COUNTER_LEN]) -> Result<(), Error> {
        let mut cloned = self.xoofff.clone();

        if plain.len() > 0 {
            self._absorb_squeeze(&mut cloned, counter, 0b0, plain);
            self._absorb_squeeze(&mut cloned, &plain.as_slice(), 0b1, tag);

        } else {
            self._absorb_squeeze(&mut cloned, counter, 0b1, tag);
        };

        Ok(())
    }

    #[inline(always)]
    pub fn wrap(&mut self, plain: &mut Vec<u8>, counter: &[u8; COUNTER_LEN]) -> Result<(), Error> {
        self._wrap(plain, counter)
    }

    #[inline(never)]
    pub fn wrap_last(mut self, plain: &mut Vec<u8>, counter: &[u8; COUNTER_LEN]) -> Result<(), Error> {
        self._wrap(plain, counter)
    }

    #[inline(always)]
    pub fn wrap_inplace_detached(&mut self, plain: &mut Vec<u8>, tag: &mut [u8; TAG_LEN], counter: &[u8; COUNTER_LEN]) -> Result<(), Error> {
        self._wrap_inplace_detached(plain, tag, counter)
    }

    #[inline(never)]
    pub fn wrap_last_inplace_detached(mut self, plain: &mut Vec<u8>, tag: &mut [u8; TAG_LEN], counter: &[u8; COUNTER_LEN]) -> Result<(), Error> {
        self._wrap_inplace_detached(plain, tag, counter)
    }

    #[inline(always)]
    fn _unwrap(&mut self, cipher: &mut Vec<u8>) -> Result<(), Error> {
        let mut cloned = self.xoofff.clone();
        let mut cloned2: Option<Xoofff> = None;
        let tag;

        if cipher.len() > COUNTER_TAG_LEN {
            let ct = &cipher[..cipher.len()-COUNTER_TAG_LEN];
            tag = &cipher[cipher.len()-TAG_LEN..];
            let counter = &cipher[cipher.len()-COUNTER_TAG_LEN..cipher.len()-TAG_LEN];
            cloned.absorb(&counter, 0b0, DS_BIT_LEN);
            cloned2 = Some(cloned.clone());

            cloned.restart();
            cloned.absorb(&ct, 0b1, DS_BIT_LEN)
        } else {
            tag = &cipher[COUNTER_LEN..];
            let counter = &cipher[..COUNTER_LEN];
            cloned.absorb(&counter, 0b1, DS_BIT_LEN);
        }

        let mut tag_prime = [0u8; TAG_LEN];
        cloned.squeeze(&mut tag_prime);

        if tag != tag_prime {
            return Err(Error::WrongTag);
        }

        if cipher.len() > COUNTER_TAG_LEN {
            cloned2.unwrap().squeeze(cipher);
        }

        cipher.truncate(cipher.len()-COUNTER_TAG_LEN);
        Ok(())
    }

    #[inline(always)]
    fn _unwrap_inplace_detached(&mut self, cipher: &mut Vec<u8>, tag: &mut [u8; TAG_LEN], counter: &[u8; COUNTER_LEN]) -> Result<(), Error> {
        let mut cloned = self.xoofff.clone();
        let mut cloned2: Option<Xoofff> = None;

        if cipher.len() > 0 {
            cloned.absorb(counter, 0b0, DS_BIT_LEN);
            cloned2 = Some(cloned.clone());

            cloned.restart();
            cloned.absorb(&cipher, 0b1, DS_BIT_LEN);
        } else {
            cloned.absorb(counter, 0b1, DS_BIT_LEN);
        }

        let mut tag_prime = [0u8; TAG_LEN];
        cloned.squeeze(&mut tag_prime);

        if *tag != tag_prime {
            return Err(Error::WrongTag);
        }

        if cipher.len() > 0 {
            cloned2.unwrap().squeeze(cipher);
        }

        Ok(())
    }

    #[inline(always)]
    pub fn unwrap(&mut self, ct: &mut Vec<u8>) -> Result<(), Error> {
        self._unwrap(ct)
    }

    #[inline(never)]
    pub fn unwrap_last(&mut self, ct: &mut Vec<u8>) -> Result<(), Error> {
        self._unwrap(ct)
    }

    #[inline(always)]
    pub fn unwrap_inplace_detached(&mut self, ct: &mut Vec<u8>, tag: &mut [u8; TAG_LEN], counter: &[u8; COUNTER_LEN]) -> Result<(), Error> {
        self._unwrap_inplace_detached(ct, tag, counter)
    }

    #[inline(never)]
    pub fn unwrap_last_inplace_detached(&mut self, ct: &mut Vec<u8>, tag: &mut [u8; TAG_LEN], counter: &[u8; COUNTER_LEN]) -> Result<(), Error> {
        self._unwrap_inplace_detached(ct, tag, counter)
    }
}
