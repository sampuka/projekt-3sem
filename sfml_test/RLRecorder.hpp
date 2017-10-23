#ifndef RLRECORDER_H
#define RLRECORDER_H

#include <SFML/Audio.hpp>
#include "DTMF_type.hpp"
#include "rc_fft.hpp"
#include <vector>
#include <complex>

class RLRecorder: public sf::SoundRecorder
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
    CArray workCArray;
    int threshold;
    sf::Clock clock;
    sf::Time sampleTime;
    std::vector<double> peakFreqs;
};

#endif
