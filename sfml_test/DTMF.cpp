#include <iostream>
#include <cmath>
#include <SFML/Audio.hpp>
#include "DTMF.hpp"
#include "rc_fft.hpp"
#include "DTMF_type.hpp"

// Windows autism
#define _USE_MATH_DEFINES
#include "math.h"

using namespace std;

DTMF::DTMF()
{
}

DTMF::~DTMF()
{
    delete recorder;
}

DTMF::DTMF(int toneLength)
{
    bufferTime = toneLength;
    double sampleInterval = (bufferTime/1000.f)/SAMPLES_PER_BUFFER;
    cout << "sampleInterval: " << sampleInterval << endl;
    
    for (unsigned int j = 0; j < sampleArray.size(); j++)
    {
	sampleArray[j] = {};
	for (int i = 0; i < SAMPLES_PER_BUFFER; i++)
	    sampleArray[j].push_back(static_cast<sf::Int16>(
		SAMPLE_AMPLITUDE/2*sin((2*M_PI * sampleFreqs[j][0] * i * sampleInterval))+
		SAMPLE_AMPLITUDE/2*sin((2*M_PI * sampleFreqs[j][1] * i * sampleInterval))
					 ));
    }

/*
    
    //Does fft on sampleArray[5] on prints all frquencies with amplitude above 50 mil (wow)
    //The frequency ascociated with a sample might be calculated wrong, but is close
    //Removable
    cout << "fft test" << endl;
    double Fs = 1/sampleInterval;
    cout << "Fs: " << Fs << endl;
    double f0 = Fs/SAMPLES_PER_BUFFER;
    CArray car;
    car.resize(SAMPLES_PER_BUFFER);
    for(int i = 0; i < SAMPLES_PER_BUFFER; i++)
	car[i] = sampleArray[5][i];
    fft(car);
    for(int i = 0; i < SAMPLES_PER_BUFFER; i++)
	if(abs(car[i]) > 50000000)
	    cout << i/bufferTime << ": " << abs(car[i]) << ", ";
    cout << endl;
*/
}

void DTMF::play(DTMF_type type)
{
    buffer.loadFromSamples(&(sampleArray[type])[0], sampleArray[type].size(), 1, SAMPLES_PER_BUFFER/bufferTime*1000);

    sound.setBuffer(buffer);
    sound.play();
}

void DTMF::play_wait(DTMF_type type)
{
    play(type);
    sf::sleep(sf::milliseconds(bufferTime));
}

void DTMF::play_list(vector<DTMF_type> toneList)
{
    for (DTMF_type tone : toneList)
	play_wait(tone);
}

void DTMF::startRecording()
{
    if (!RLRecorder::isAvailable())
        cout << "no mic available" << endl;
    else
	cout << "mic available" << endl;

    recorder = new RLRecorder(&currentTone);
    recorder->start(50000);
}

DTMF_type DTMF::listen()
{
    return currentTone;
}
