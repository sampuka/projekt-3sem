#ifndef DTMF_H
#define DTMF_H

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <array>

// Number of samples for a buffer
#define SAMPLES_PER_BUFFER 10000

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
};

class DTMF
{
public:
    DTMF(int toneLength);
    void play(DTMF_type type);
    void play_wait(DTMF_type type);
private:
    int sampleTime; // Length of tone in milliseconds
    sf::Sound sound;
    sf::SoundBuffer buffer;
    std::array<std::vector<sf::Int16>, 16> sampleArray;
};

#endif
