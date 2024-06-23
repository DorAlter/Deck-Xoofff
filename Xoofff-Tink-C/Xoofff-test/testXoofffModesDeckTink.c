/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "config.h"
#ifdef XKCP_has_Xoofff

#include <stdlib.h>
#include <time.h>
#include "KeccakSponge.h"
#include "XoofffModes.h"
#include "UT.h"

#if defined(XoodooSmallRAM)
    #define    XoodooSizeMultiplier    2
#else
    #define    XoodooSizeMultiplier    16
#endif

#define SnP_width_sponge        1600
#define SnP_width               (SnP_widthInBytes*8)
#define dataByteSize            (2*XoodooSizeMultiplier*SnP_widthInBytes+SnP_widthInBytes)
#define keyByteSize             (1*SnP_widthInBytes)
#define nonceByteSize           (2*SnP_widthInBytes)
#define WByteSize               (2*SnP_widthInBytes)
#define dataBitSize             (dataByteSize*8)
#define keyBitSize              (keyByteSize*8)
#define nonceBitSize            (nonceByteSize*8)
#define WBitSize                (WByteSize*8)
#define tagLenDeckTink      32

#define checksumByteSize        16

static void assert(int condition)
{
    UT_assert(condition, "");
}

static void randomize( unsigned char* data, unsigned int length)
{
    #if !defined(UT_EMBEDDED)
    srand((unsigned int)time(0));
    #endif
    while (length--)
    {
        *data++ = (unsigned char)rand();
    }
}

static void generateSimpleRawMaterial(unsigned char* data, size_t length, unsigned char seed1, unsigned int seed2)
{
    size_t i;

    for(i=0; i<length; i++) {
        unsigned char iRolled;
        unsigned char byte;
        seed2 = seed2 % 8;
        iRolled = ((unsigned char)i << seed2) | ((unsigned char)i >> (8-seed2));
        byte = seed1 + 161* (unsigned char)length - iRolled + (unsigned char)i;
        data[i] = byte;
    }
}

#if defined(UT_OUTPUT)
static void outputHex(const unsigned char *data, unsigned char length)
{
    unsigned int i;
    for(i=0; i<length; i++)
        printf("%02x ", (int)data[i]);
    printf("\n\n");
}
#endif


/* ------------------------------------------------------------------------- */


static void performTestDeckTink_OneInput(BitLength keyLen, BitLength nonceLen, BitLength dataLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum)
{
    BitSequence input[dataByteSize];
    BitSequence inputPrime[dataByteSize];
    BitSequence output[dataByteSize];
    BitSequence key[keyByteSize];
    BitSequence nonce[nonceByteSize];
    unsigned char tag[tagLenDeckTink];
    unsigned char tagInit[tagLenDeckTink];
    Deck_Tink_Instance kvEnc;
    Deck_Tink_Instance kvDec;
    int result;
    unsigned int seed;
    unsigned int session;
    unsigned int isLast = 0;

    randomize((unsigned char *)&kvEnc, sizeof(Deck_Tink_Instance));
    randomize((unsigned char *)&kvDec, sizeof(Deck_Tink_Instance));
    randomize(key, keyByteSize);
    randomize(nonce, nonceByteSize);
    randomize(input, dataByteSize);
    randomize(inputPrime, dataByteSize);
    randomize(output, dataByteSize);
    randomize(tag, tagLenDeckTink);

    seed = (unsigned int)keyLen + (unsigned int)nonceLen + (unsigned int)dataLen;
    seed ^= seed >> 3;
    generateSimpleRawMaterial(key, (keyLen + 7) / 8, (unsigned char)(0x4321 - seed), 0x89 + seed);
    if (keyLen & 7)
        key[keyLen / 8] &= (1 << (keyLen & 7)) - 1;
    generateSimpleRawMaterial(nonce, (nonceLen + 7) / 8, (unsigned char)(0x6523 - seed), 0x43 + seed);
    if (nonceLen & 7)
        nonce[nonceLen / 8] &= (1 << (nonceLen & 7)) - 1;
    generateSimpleRawMaterial(input, (dataLen + 7) / 8, (unsigned char)(0x1A29 - seed), 0xC3 + seed);
    if (dataLen & 7)
        input[dataLen / 8] &= (1 << (dataLen & 7)) - 1;

    #ifdef UT_VERBOSE
    printf( "keyLen %5u, nonceLen %5u, dataLen %5u (in bits)\n", (unsigned int)keyLen, (unsigned int)nonceLen, (unsigned int)dataLen);
    #endif

    result = Deck_Tink_Initialize(&kvEnc, key, keyLen, nonce, nonceLen);
    assert(result == 0);
    result = Deck_Tink_Initialize(&kvDec, key, keyLen, nonce, nonceLen);
    assert(result == 0);

    #ifdef UT_VERBOSE
    {
        unsigned int i;
        unsigned int len;

        printf("Key of %d bits:", (int)keyLen);
        len = (keyLen + 7) / 8;
        for(i=0; (i<len) && (i<16); i++)
            printf(" %02x", (int)key[i]);
        if (len > 16)
            printf(" ...");
        printf("\n");

        printf("Nonce of %d bits:", (int)nonceLen);
        len = (nonceLen + 7) / 8;
        for(i=0; (i<len) && (i<16); i++)
            printf(" %02x", (int)nonce[i]);
        if (len > 16)
            printf(" ...");
        printf("\n");

        printf("Input of %d bits:", (int)dataLen);
        len = (dataLen + 7) / 8;
        for(i=0; (i<len) && (i<16); i++)
            printf(" %02x", (int)input[i]);
        if (len > 16)
            printf(" ...");
        printf("\n");

    }
    #endif

    for (session = 3; session != 0; --session) {
        unsigned int counter;
        result = Deck_Tink_Wrap(&kvEnc, input, output, dataLen, tag, isLast, counter);
        assert(result == 0);
        result = Deck_Tink_Unwrap(&kvDec, output, inputPrime, dataLen + tagLenDeckTink * 8, tag, isLast, counter);
        assert(result == 0);
        assert(!memcmp(input,inputPrime,(dataLen + 7) / 8));
        KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, (dataLen + 7) / 8);
        KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, tag, tagLenDeckTink);
        #ifdef UT_VERBOSE
        {
            unsigned int i;
            unsigned int len;

            printf("Output of %d bits:", (int)dataLen);
            len = (dataLen + 7) / 8;
            for(i=0; (i<len) && (i<8); i++)
                printf(" %02x", (int)output[i]);
            if (len > 16)
                printf(" ...");
            if (i < (len - 8))
                i = len - 8;
            for( /* empty */; i<len; i++)
                printf(" %02x", (int)output[i]);
            printf("\n");

            printf("Tag of %d bytes:", (int)tagLenDeckTink);
            for(i=0; i<tagLenDeckTink; i++)
                printf(" %02x", (int)tag[i]);
            printf("\n");
            fflush(stdout);
            if (session == 1)
                printf("\n");
        }
        #endif
    }

}


static void performTestDeckTink(unsigned char *checksum)
{
    BitLength dataLen, keyLen, nonceLen;

    /* Accumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width_sponge, 0);

    #ifdef UT_VERBOSE
    printf("k ");
    #endif
    dataLen = 128*8;
    nonceLen = 24*8;
    for(keyLen=0; keyLen<keyBitSize; keyLen = (keyLen < 2*SnP_width) ? (keyLen+1) : (keyLen+8)) {
        performTestDeckTink_OneInput(keyLen, nonceLen, dataLen, &spongeChecksum);
    }
    
    #ifdef UT_VERBOSE
    printf("n ");
    #endif
    dataLen = 128*8;
    keyLen = 16*8;
    for(nonceLen=0; nonceLen<=nonceBitSize; nonceLen = (nonceLen < 2*SnP_width) ? (nonceLen+1) : (nonceLen+8)) {
        performTestDeckTink_OneInput(keyLen, nonceLen, dataLen, &spongeChecksum);
    }
    
    #ifdef UT_VERBOSE
    printf("d ");
    #endif
    keyLen = 16*8;
    nonceLen = 24*8;
    for(dataLen=0; dataLen<=dataBitSize; dataLen = (dataLen < 2*SnP_width) ? (dataLen+1) : (dataLen+8)) {
        performTestDeckTink_OneInput(keyLen, nonceLen, dataLen, &spongeChecksum);
    }
    
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef UT_VERBOSE
    {
        unsigned int i;
        printf("Xoofff-SANE\n" );
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}


void selfTestDeckTink(const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];

    UT_startTest("Deck-Tink", "");
    performTestDeckTink(checksum);
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
    UT_endTest();
}

#ifdef UT_OUTPUT
void writeTestXoofffSANSE_One(FILE *f)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    printf("Writing Xoofff-SANSE ");
    performTestXoofffSANSE(checksum);
    fprintf(f, "    selfTestXoofffSANSE(\"");
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
    printf("\n");
}

void writeTestXoofffSANSE(const char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f != NULL);
    writeTestXoofffSANSE_One(f);
    fclose(f);
}
#endif


void testXoofffModes(void)
{
    selfTestDeckTink((const unsigned char *)"\x06\xed\xf9\xa6\x70\xb3\xfe\x83\x34\x2c\xb4\x18\x75\x0d\xf2\xcc");

}
#endif /* XKCP_has_Xoofff */