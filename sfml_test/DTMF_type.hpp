#ifndef DTMF_TYPES_H
#define DTMF_TYPES_H

// Number of samples for a buffer
#define SAMPLES_PER_BUFFER 50000

// The amplitude of the samples. OPS: signed 16-bit int size
#define SAMPLE_AMPLITUDE 15000

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

#endif
