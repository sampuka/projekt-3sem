#ifndef DTMF_H
#define DTMF_H

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <array>

// Number of samples for a buffer
#define SAMPLES_PER_BUFFER 100000

// The amplitude of the samples. OPS: signed 16-bit int size
#define SAMPLE_AMPLITUDE 15000

enum DTMF_type
{
    DTMF_A,
    DTMF_B,
    DTMF_C
};

const int sampleFreqs[3] = {800, 2000, 1400};

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
    std::array<std::vector<sf::Int16>, 3> sampleArray;
};

#endif
