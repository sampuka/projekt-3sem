#include <SFML/Audio.hpp>
#include "RLRecorder.hpp"
#include "rc_fft.hpp"

using namespace std;

RLRecorder::RLRecorder()
{
}

RLRecorder::RLRecorder(DTMF_type *_currentTone)
{
    currentTone = _currentTone;
    workCArray = {};
}

bool RLRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
    //workCArray.reserve(sampleCount);
    copy(samples, &samples[sampleCount], &workCArray[0]);
    
    fft(workCArray);
    return true;
}
