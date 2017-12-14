#ifndef RLRECORDER_H
#define RLRECORDER_H

#include <SFML/Audio.hpp>
#include "DTMF_type.hpp"
#include <vector>
#include <complex>

#define FS 50000.0 //Make sure it's the same as in DTMF.cpp

class RLRecorder : public sf::SoundRecorder
{
public:
    RLRecorder();
    ~RLRecorder();
    RLRecorder(DTMF_type *_currentTone);
    bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override;
    bool onStart() override;
    void onStop() override;

private:
    void updateCurrentTone();
    DTMF_type *currentTone;
    //CArray workCArray;
    float workFArray[50000]; //EXTREME WARNING RIGHT HERE CANNOT HANDLE IF SAMPLECOUNT IS HIGHER. THIS IS WHY THE PROGRAM CRASHES :oooouuu
    float freq_mags[8];
    short DTMF_byte;
    //int threshold;
    float upperthreshold;
    float lowerthreshold;
    sf::Clock clock;
    sf::Time sampleTime;
    //std::vector<double> peakFreqs;
};

#endif
