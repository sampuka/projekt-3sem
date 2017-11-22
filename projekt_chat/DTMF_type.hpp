#ifndef DTMF_TYPES_H
#define DTMF_TYPES_H

#include <iostream>

// Number of samples for a buffer
#define SAMPLES_PER_BUFFER 50000

// The amplitude of the samples. OPS: signed 16-bit int size
#define SAMPLE_AMPLITUDE 15000

// https://en.wikipedia.org/wiki/Dual-tone_multi-frequency_signaling


const int DTMFFreqs[8] =
{
    697,
    770,
    852,
    941,
    1209,
    1336,
    1447,
    1633
};

const int sampleFreqs[16][2] =
{
    {DTMFFreqs[4], DTMFFreqs[0]},  // 1
    {DTMFFreqs[5], DTMFFreqs[0]},  // 2
    {DTMFFreqs[6], DTMFFreqs[0]},  // 3
    {DTMFFreqs[7], DTMFFreqs[0]},  // A
    {DTMFFreqs[4], DTMFFreqs[1]},  // 4
    {DTMFFreqs[5], DTMFFreqs[1]},  // 5
    {DTMFFreqs[6], DTMFFreqs[1]},  // 6
    {DTMFFreqs[7], DTMFFreqs[1]},  // B
    {DTMFFreqs[4], DTMFFreqs[2]},  // 1
    {DTMFFreqs[5], DTMFFreqs[2]},  // 2
    {DTMFFreqs[6], DTMFFreqs[2]},  // 3
    {DTMFFreqs[7], DTMFFreqs[2]},  // C
    {DTMFFreqs[4], DTMFFreqs[3]},  // STAR
    {DTMFFreqs[5], DTMFFreqs[3]},  // 0
    {DTMFFreqs[6], DTMFFreqs[3]},  // HASH
    {DTMFFreqs[7], DTMFFreqs[3]},  // D
};

enum DTMF_type
{
    DTMF_1,
    DTMF_2,
    DTMF_3,
    DTMF_A,
    DTMF_4,
    DTMF_5,
    DTMF_6,
    DTMF_B,
    DTMF_7,
    DTMF_8,
    DTMF_9,
    DTMF_C,
    DTMF_STAR,
    DTMF_0,
    DTMF_HASH,
    DTMF_D,
    DTMF_UNKNOWN //Needs a non-value
};

std::ostream& operator<< (std::ostream& os, const DTMF_type& dtmf);

#endif
