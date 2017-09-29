#include <iostream>
#include <cmath>
#include <SFML/Audio.hpp>
#include "DTMF.hpp"

// Windows autism
#define _USE_MATH_DEFINES
#include "math.h"

using namespace std;

const int sampleFreqs[16][2] = {
    {1209, 697},  // 1
    {1336, 697},  // 2
    {1447, 697},  // 3
    {1633, 697},  // A
    {1209, 770},  // 4
    {1336, 770},  // 5
    {1447, 770},  // 6
    {1633, 770},  // B
    {1209, 852},  // 7
    {1336, 852},  // 8
    {1447, 852},  // 9
    {1633, 852},  // C
    {1209, 941},  // STAR
    {1336, 941},  // 0
    {1447, 941},  // HASH
    {1633, 941}   // D
};

DTMF::DTMF(int toneLength)
{
    sampleTime = toneLength;
    
    for (unsigned int j = 0; j < sampleArray.size(); j++)
    {
	sampleArray[j] = {};
	for (int i = 0; i < SAMPLES_PER_BUFFER; i++)
	    sampleArray[j].push_back(static_cast<sf::Int16>(
		SAMPLE_AMPLITUDE/2*sin((2*M_PI * sampleFreqs[j][0] * i * ((sampleTime/(float)1000)/(float)SAMPLES_PER_BUFFER)))+
		SAMPLE_AMPLITUDE/2*sin((2*M_PI * sampleFreqs[j][1] * i * ((sampleTime/(float)1000)/(float)SAMPLES_PER_BUFFER)))
					 ));
    }
}

void DTMF::play(DTMF_type type)
{
    buffer.loadFromSamples(&(sampleArray[type])[0], sampleArray[type].size(), 1, SAMPLES_PER_BUFFER/sampleTime*1000);

    sound.setBuffer(buffer);
    sound.play();
}

void DTMF::play_wait(DTMF_type type)
{
    play(type);
    sf::sleep(sf::milliseconds(sampleTime));
}

void DTMF::play_list(vector<DTMF_type> toneList)
{
    for (DTMF_type tone : toneList)
	play_wait(tone);
}
