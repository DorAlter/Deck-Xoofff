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

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#ifdef XKCP_has_Xoofff
#include "Xoofff.h"
#include "XoofffModes.h"
#endif
#ifdef XKCP_has_Xoodyak
#include "Xoodyak.h"
#endif
#include "timing.h"
#include "testPerformance.h"
#include "testXooPerformance.h"

typedef cycles_t (* measurePerf)(cycles_t, unsigned int);

void displayMeasurements1101001000(cycles_t *measurements, uint32_t *laneCounts, unsigned int numberOfColumns, unsigned int laneLengthInBytes);

#define xstr(s) str(s)
#define str(s) #s


#ifdef XKCP_has_Xoodoo
#include "Xoodoo-SnP.h"

    #define prefix Xoodoo
    #define SnP Xoodoo
    #define SnP_width 384
    #define SnP_Permute_6rounds  Xoodoo_Permute_6rounds
    #define SnP_Permute_12rounds Xoodoo_Permute_12rounds
        #include "timingXooSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
#endif

#ifdef XKCP_has_Xoodootimes4
#include "Xoodoo-times4-SnP.h"
#if !defined(Xoodootimes4_isFallback)

    #define prefix                      Xoodootimes4
    #define PlSnP                       Xoodootimes4
    #define PlSnP_parallelism           4
    #define SnP_width                   384
    #define PlSnP_PermuteAll_6rounds    Xoodootimes4_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes4_PermuteAll_12rounds
        #include "timingXooPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
#endif
#endif

#ifdef XKCP_has_Xoodootimes8
#include "Xoodoo-times8-SnP.h"
#if !defined(Xoodootimes8_isFallback)

    #define prefix                      Xoodootimes8
    #define PlSnP                       Xoodootimes8
    #define PlSnP_parallelism           8
    #define SnP_width                   384
    #define PlSnP_PermuteAll_6rounds    Xoodootimes8_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes8_PermuteAll_12rounds
        #include "timingXooPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
#endif
#endif

#ifdef XKCP_has_Xoodootimes16
#include "Xoodoo-times16-SnP.h"
#if !defined(Xoodootimes16_isFallback)

    #define prefix                      Xoodootimes16
    #define PlSnP                       Xoodootimes16
    #define PlSnP_parallelism           16
    #define SnP_width                   384
    #define PlSnP_PermuteAll_6rounds    Xoodootimes16_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes16_PermuteAll_12rounds
        #include "timingXooPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
#endif
#endif

#ifdef XKCP_has_Xoofff

#define Xoofff_KeyLen 16
#define Xoofff_rate   384

cycles_t measureXoofff_MaskDerivation(cycles_t dtMin)
{
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    Xoofff_Instance xp;
 
    memset(key, 0xA5, Xoofff_KeyLen);

    {
        measureTimingBegin
        Xoofff_MaskDerivation(&xp, key, Xoofff_KeyLen*8);
        measureTimingEnd
    }
}

cycles_t measureXoofff_Compress(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    Xoofff_Instance xp;
 
    assert(inputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Xoofff_KeyLen);
    Xoofff_MaskDerivation(&xp, key, Xoofff_KeyLen*8);
    memset(input, 0xA5, inputLen/8);

    {
        measureTimingBegin
        Xoofff_Compress(&xp, input, inputLen, Xoofff_FlagLastPart);
        measureTimingEnd
    }
}

cycles_t measureXoofff_Expand(cycles_t dtMin, unsigned int outputLen)
{
    unsigned char* output = bigBuffer1;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    Xoofff_Instance xp;

    assert(outputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Xoofff_KeyLen);
    Xoofff_MaskDerivation(&xp, key, Xoofff_KeyLen*8);
    output[0] = 0;
    Xoofff_Compress(&xp, output, 0, Xoofff_FlagLastPart);

    {
        measureTimingBegin
        xp.phase = COMPRESSING; /* avoid error when calling multiple times with last flag set */
        Xoofff_Expand(&xp, output, outputLen, Xoofff_FlagLastPart);
        measureTimingEnd
    }
}

cycles_t measureDeckTink_Init(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    unsigned char* output = bigBuffer2;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char nonce[16];
    ALIGN(64) unsigned char tag[Deck_Tink_TagLength];
    Deck_Tink_Instance xp;

    assert(inputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Xoofff_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));

    {
        measureTimingBegin
        Deck_Tink_Initialize(&xp, key, Xoofff_KeyLen*8, nonce, sizeof(nonce)*8);
        measureTimingEnd
    }
}

cycles_t measureDeckTink_Wrap(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    unsigned char* output = bigBuffer2;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char nonce[16];
    ALIGN(64) unsigned char tag[Deck_Tink_TagLength];
    BitSequence counter[8] = {0};
    Deck_Tink_Instance xp;

    assert(inputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Xoofff_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));
    Deck_Tink_Initialize(&xp, key, Xoofff_KeyLen*8, nonce, sizeof(nonce)*8);
    memset(input, 0xA5, inputLen/8);

    {
        measureTimingBegin
        Deck_Tink_Wrap(&xp, input, output, inputLen, tag, 0, counter);
        measureTimingEnd
    }
}

cycles_t measureDeckTink_Mac(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    unsigned char* output = bigBuffer2;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char nonce[16];
    ALIGN(64) unsigned char tag[Deck_Tink_TagLength];
    BitSequence counter[8] = {0};
    Deck_Tink_Instance xp;

    assert(inputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Xoofff_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));
    Deck_Tink_Initialize(&xp, key, Xoofff_KeyLen*8, nonce, sizeof(nonce)*8);
    memset(input, 0xA5, inputLen/8);
    

    {
        measureTimingBegin
        Deck_Tink_Wrap(&xp, input, output, 0, tag, 0, counter);
        measureTimingEnd
    }
}

cycles_t measureDeckTink_Unwrap(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    unsigned char* output = bigBuffer2;
    unsigned char* inputPrime = bigBuffer3;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char nonce[16];
    ALIGN(64) unsigned char tag[Deck_Tink_TagLength];
    BitSequence counter[8] = {0};
    Deck_Tink_Instance kvEnc;
    Deck_Tink_Instance kvDec;

    assert(inputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Xoofff_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));
    Deck_Tink_Initialize(&kvEnc, key, Xoofff_KeyLen*8, nonce, sizeof(nonce)*8);
    Deck_Tink_Initialize(&kvDec, key, Xoofff_KeyLen*8, nonce, sizeof(nonce)*8);
    memset(input, 0xA5, inputLen/8);
    Deck_Tink_Wrap(&kvEnc, input, output, inputLen, tag, 0, counter);    

    {
        measureTimingBegin
        Deck_Tink_Unwrap(&kvDec, output, inputPrime, inputLen + Deck_Tink_TagLength*8, tag, 0, counter);
        measureTimingEnd
    }
}

cycles_t measureXoofffSANE_Wrap(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    unsigned char* output = bigBuffer2;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char nonce[16];
    ALIGN(64) unsigned char AD[16];
    ALIGN(64) unsigned char tag[XoofffSANE_TagLength];
    XoofffSANE_Instance xp;

    assert(inputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Xoofff_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));
    XoofffSANE_Initialize(&xp, key, Xoofff_KeyLen*8, nonce, sizeof(nonce)*8, tag);
    memset(input, 0xA5, inputLen/8);
    memset(AD, 0x5A, sizeof(AD));

    {
        measureTimingBegin
        XoofffSANE_Wrap(&xp, input, output, inputLen, AD, 0, tag);
        measureTimingEnd
    }
}

cycles_t measureXoofffSANE_MAC(cycles_t dtMin, unsigned int ADLen)
{
    ALIGN(64) unsigned char input[1];
    ALIGN(64) unsigned char output[1];
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char nonce[16];
    unsigned char* AD = bigBuffer1;
    ALIGN(64) unsigned char tag[XoofffSANE_TagLength];
    XoofffSANE_Instance xp;

    assert(ADLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Xoofff_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));
    XoofffSANE_Initialize(&xp, key, Xoofff_KeyLen*8, nonce, sizeof(nonce)*8, tag);
    memset(input, 0xA5, sizeof(input));
    memset(AD, 0x5A, ADLen/8);

    {
        measureTimingBegin
        XoofffSANE_Wrap(&xp, input, output, 0, AD, ADLen, tag);
        measureTimingEnd
    }
}


void printXoofffPerformanceHeader( void )
{
    printf("*** Xoofff ***\n");
    printf("Using Xoodoo implementations:\n");
    printf("- \303\227\x31: " Xoodoo_implementation "\n");
    #if defined(XKCP_has_Xoodootimes4) && !defined(Xoodootimes4_isFallback)
    printf("- \303\2274: " Xoodootimes4_implementation "\n");
    #endif
    #if defined(XKCP_has_Xoodootimes8) && !defined(Xoodootimes8_isFallback)
    printf("- \303\2278: " Xoodootimes8_implementation "\n");
    #endif
    #if defined(XKCP_has_Xoodootimes16) && !defined(Xoodootimes16_isFallback)
    printf("- \303\22716: " Xoodootimes16_implementation "\n");
    #endif
    printf("\n");
}

uint32_t testXoofffNextLen( uint32_t len )
{
    if (len < Xoofff_rate) {
        len <<= 1;
        if (len > Xoofff_rate)
            len = Xoofff_rate;
    }
    else
        len <<= 1;
    return len;
}

uint32_t testXoofffAdaptLen( uint32_t len )
{
    return (len < Xoofff_rate) ? len : (len-8);
}

static void testXooPerfSlope( measurePerf pFunc, cycles_t calibration )
{
    uint32_t len;
    uint32_t count;
    cycles_t time;
    cycles_t time128;
    cycles_t time256;
    const uint32_t stabilityCount = 10;

    time128 = CYCLES_MAX;
    len = 128*Xoofff_rate;
    count = stabilityCount;
    do {
        time = pFunc(calibration, len);
        if (time < time128) {
            time128 = time;
            count = stabilityCount;
        }
    } while( --count != 0);
    time256 = CYCLES_MAX;
    len = 256*Xoofff_rate;
    count = stabilityCount;
    do {
        time = pFunc(calibration, len);
        if (time < time256) {
            time256 = time;
            count = stabilityCount;
        }
    } while( --count != 0);

    time = time256-time128;
    len = 128*Xoofff_rate;
    printf("Slope %8d bytes (%u blocks): %9"PRId64" %s, %6.3f %s/byte\n", len/8, len/Xoofff_rate, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
}

void testXoofffPerformanceOne( void )
{
    cycles_t calibration = CalibrateTimer();
    uint32_t len;
    cycles_t time;

    printf("Xoofff_Compress\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofff_Compress(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofff_Compress(calibration, len);
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureXoofff_Compress, calibration);
    
    printf("\nXoofff_Expand\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofff_Expand(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofff_Expand(calibration, len);
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureXoofff_Expand, calibration);

    printf("\nDeck-Tink Initialization \n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureDeckTink_Init(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureDeckTink_Init(calibration, len);
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureDeckTink_Init, calibration);

    printf("\nDeck-Tink Wrap \n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureDeckTink_Wrap(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureDeckTink_Wrap(calibration, len);
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureDeckTink_Wrap, calibration);

    printf("\nDeck-Tink MAC (wrap with no plaintext)\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureDeckTink_Mac(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureDeckTink_Mac(calibration, len);
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureDeckTink_Mac, calibration);

    printf("\nDeck-Tink Unwrap \n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureDeckTink_Unwrap(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureDeckTink_Unwrap(calibration, len);
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureDeckTink_Unwrap, calibration);

    printf("\nXoofffSANE Wrap\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofffSANE_Wrap(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofffSANE_Wrap(calibration, len);
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureXoofffSANE_Wrap, calibration);

    printf("\nXoofffSANE MAC\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofffSANE_MAC(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofffSANE_MAC(calibration, len);
        printf("%8d bytes: %9"PRId64" %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureXoofffSANE_MAC, calibration);

    printf("\n\n");
}

void testXoofffPerformance(void)
{
    printXoofffPerformanceHeader();
    testXoofffPerformanceOne();
}
#endif

void testXooPerformance(void)
{

#ifdef XKCP_has_Xoodoo
    Xoodoo_timingSnP("Xoodoo", Xoodoo_implementation);
#endif
#if defined(XKCP_has_Xoodootimes4) && !defined(Xoodootimes4_isFallback)
    Xoodootimes4_timingPlSnP("Xoodoo\303\2274", Xoodootimes4_implementation);
#endif
#if defined(XKCP_has_Xoodootimes8) && !defined(Xoodootimes8_isFallback)
    Xoodootimes8_timingPlSnP("Xoodoo\303\2278", Xoodootimes8_implementation);
#endif
#if defined(XKCP_has_Xoodootimes16) && !defined(Xoodootimes16_isFallback)
    Xoodootimes16_timingPlSnP("Xoodoo\303\22716", Xoodootimes16_implementation);
#endif

#ifdef XKCP_has_Xoofff
    testXoofffPerformance();
#endif
#ifdef XKCP_has_Xoodyak
    testXoodyakPerformance();
#endif
}