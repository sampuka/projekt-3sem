#include <iostream>
#include <SFML/Audio.hpp>
#include "RLRecorder.hpp"
#include "rc_fft.hpp"

using namespace std;

void print_CArray(const CArray vec)
{
    unsigned int len = vec.size();
    cout << '{';
    for(unsigned int i = 0; i < len; i++)
	cout << vec[i].real() << ", ";
    cout << '}' << endl;
}

RLRecorder::RLRecorder()
{
}

RLRecorder::~RLRecorder()
{
    stop();
}

RLRecorder::RLRecorder(DTMF_type *_currentTone)
{
    currentTone = _currentTone;
    workCArray = {1, 0, -1, 0, 1, 0, -1, 0};
    cout << "fft test" << endl;
    print_CArray(workCArray);
    fft(workCArray);
    print_CArray(workCArray);
}

bool RLRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
    cout << "sampleCount: " << sampleCount << endl;

    workCArray.resize(static_cast<long unsigned int>(sampleCount));

    for(std::size_t i = 0; i < sampleCount; i++)
	workCArray[i] = samples[i];

    print_CArray(workCArray);
    fft(workCArray);
    print_CArray(workCArray);
    return true;
}

bool RLRecorder::onStart()
{
    return true;
}

void RLRecorder::onStop()
{
}
