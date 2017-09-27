#include <iostream>
#include <cmath>
#include <SFML/Audio.hpp>
#include "DTMF.hpp"

using namespace std;

DTMF::DTMF(int toneLength)
{
    sampleTime = toneLength;
    
    for (unsigned int j = 0; j < sampleArray.size(); j++)
    {
	sampleArray[j] = {};
	for (int i = 0; i < SAMPLES_PER_BUFFER; i++)
	    sampleArray[j].push_back(
		SAMPLE_AMPLITUDE*sin((2*M_PI * sampleFreqs[j] * i * ((sampleTime/(float)1000)/(float)SAMPLES_PER_BUFFER))));
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
