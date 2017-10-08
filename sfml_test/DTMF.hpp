#ifndef DTMF_H
#define DTMF_H

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "RLRecorder.hpp"
#include "DTMF_type.hpp"

#include <vector>
#include <array>

class DTMF
{
public:
    DTMF(int toneLength);
    void play(DTMF_type type);
    void play_wait(DTMF_type type);
    void play_list(std::vector<DTMF_type> toneList);
    void start_recording();
    DTMF_type listen();
    
private:
    int sampleTime; // Length of tone in milliseconds
    sf::Sound sound;
    sf::SoundBuffer buffer;
    std::array<std::vector<sf::Int16>, 16> sampleArray;
    DTMF_type currentTone;
    RLRecorder* recorder;
};

#endif
