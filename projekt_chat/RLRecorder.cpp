#include <iostream>
#include <array>
#include <bitset>
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
    peakFreqs = {};
    currentTone = _currentTone;
    workCArray = {1, 0, -1, 0, 1, 0, -1, 0};
    cout << "fft test" << endl;
    print_CArray(workCArray);
    fft(workCArray);
    print_CArray(workCArray);
    
    setProcessingInterval(sf::milliseconds(50));
}

bool RLRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
    sampleTime = clock.restart();
    //cout << "sampleTime: " << sampleTime.asSeconds() << endl;
    //cout << "sampleCount: " << sampleCount << endl;

    workCArray.resize(static_cast<long unsigned int>(sampleCount));

    for(std::size_t i = 0; i < sampleCount; i++)
	workCArray[i] = samples[i];

    fft(workCArray);
    applyHammingWindow(workCArray);

    int max = 0;
    for (Complex c : workCArray)
	if(abs(c) > max)
	    max = abs(c);

    threshold = 0.25*max;

    
    //cout << "threshold: " << threshold << " | 0th value: " << abs(workCArray[0])<< endl;

    peakFreqs = {};

    for(std::size_t i = 0; i < sampleCount/2; i++) //Division by 2!!! excluding the mirrored freqencies
	if(abs(workCArray[i]) > threshold)
	    peakFreqs.push_back(i/sampleTime.asSeconds());
    
    /*
    for(std::size_t i = 0; i < sampleCount; i++)
	if(abs(workCArray[i]) > threshold)
	    cout << i/sampleTime.asSeconds() << ", ";
    cout << endl;
    */
    updateCurrentTone();
    return true;
}

bool RLRecorder::onStart()
{
    return true;
}

void RLRecorder::onStop()
{
}

void RLRecorder::updateCurrentTone()
{
    int DTMF_byte = 0;
    
    array<double, 8> DTMF_tones;
    int max = 0;
    int min = 2147483647; //INT_MAX

    /*
    for (int i = 0; i < 8; i++)
    {
	DTMF_tones[i] = abs(workCArray[DTMFFreqs[i]*sampleTime.asSeconds()]);
	if(DTMF_tones[i] > max)
	    max = DTMF_tones[i];
	if(DTMF_tones[i] < min)
	    min = DTMF_tones[i];
    }
    threshold = 0.50*max;
    cout << "threshold: " << threshold << " | max-min: " << max-min << " | max/min: " << max/min << endl;
    */
    /*
    for(int i = 0; i < 8; i++)
	cout << i << ": " << DTMF_tones[i] << " | ";

    cout << endl;

    for(int i = 0; i < 8; i++)
    {
	if(DTMF_tones[i] > threshold)
	    DTMF_byte |= 1<<i;
    }

    if(max-min < 80000)
	DTMF_byte = 0;
*/

    for(double freq : peakFreqs)
	cout << freq << ", ";
    cout << endl;


    for(double freq : peakFreqs)
	for(int i = 0; i < 8; i++)
	    if(abs(freq-DTMFFreqs[i]) < 35)
		DTMF_byte |= 1<<i;

    //cout << "DTMF_byte: " << bitset<8>(DTMF_byte).to_string() << endl;

    switch(DTMF_byte)
    {
	
    case (1<<4) + (1<<0):
	*currentTone = DTMF_1;
	cout << "DTMF_1" << endl;
	break;

    case (1<<5) + (1<<0):
	*currentTone = DTMF_2;
	cout << "DTMF_2" << endl;
	break;
	
    case (1<<6) + (1<<0):
	*currentTone = DTMF_3;
	cout << "DTMF_3" << endl;
	break;
	
    case (1<<7) + (1<<0):
	*currentTone = DTMF_A;
	cout << "DTMF_A" << endl;
	break;
	
    case (1<<4) + (1<<1):
	*currentTone = DTMF_4;
	cout << "DTMF_4" << endl;
	break;

    case (1<<5) + (1<<1):
	*currentTone = DTMF_5;
	cout << "DTMF_5" << endl;
	break;
	
    case (1<<6) + (1<<1):
	*currentTone = DTMF_6;
	cout << "DTMF_6" << endl;
	break;
	
    case (1<<7) + (1<<1):
	*currentTone = DTMF_B;
	cout << "DTMF_B" << endl;
	break;
	
    case (1<<4) + (1<<2):
	*currentTone = DTMF_7;
	cout << "DTMF_7" << endl;
	break;

    case (1<<5) + (1<<2):
	*currentTone = DTMF_8;
	cout << "DTMF_8" << endl;
	break;
	
    case (1<<6) + (1<<2):
	*currentTone = DTMF_9;
	cout << "DTMF_9" << endl;
	break;
	
    case (1<<7) + (1<<2):
	*currentTone = DTMF_C;
	cout << "DTMF_C" << endl;
	break;
	
    case (1<<4) + (1<<3):
	*currentTone = DTMF_STAR;
	cout << "DTMF_STAR" << endl;
	break;

    case (1<<5) + (1<<3):
	*currentTone = DTMF_0;
	cout << "DTMF_0" << endl;
	break;
	
    case (1<<6) + (1<<3):
	*currentTone = DTMF_HASH;
	cout << "DTMF_HASH" << endl;
	break;
	
    case (1<<7) + (1<<3):
	*currentTone = DTMF_D;
	cout << "DTMF_D" << endl;
	break;
	
    default:
	*currentTone = DTMF_UNKNOWN;
	cout << "DTMF_UNKNOWN" << endl;
	break;
    }
}
