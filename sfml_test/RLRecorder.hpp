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
    RLRecorder(DTMF_type *_currentTone);
    virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);

private:
    DTMF_type *currentTone;
    CArray workCArray;
};

#endif
