#include <iostream>
#include <array>
#include <bitset>
#include <SFML/Audio.hpp>
#include "RLRecorder.hpp"
#include "rc_fft.hpp"
#include "goertzel.hpp"

#define SAMPLE_INTERVAL 25.0 //ms

using namespace std;

void print_CArray(const CArray vec)
{
    unsigned int len = vec.size();
    cout << '{';
    for (unsigned int i = 0; i < len; i++)
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
    
    peakFreqs.clear();
    currentTone = _currentTone;
    /*
      workCArray = { 1, 0, -1, 0, 1, 0, -1, 0 };
      cout << "fft test" << endl;
      print_CArray(workCArray);
      fft(workCArray);
      print_CArray(workCArray);
    */
    setProcessingInterval(sf::milliseconds(SAMPLE_INTERVAL));
}

bool RLRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
    sampleTime = sf::seconds(sampleCount / (FS));
    //cout << "sampleTime: " << sampleTime.asMilliseconds() << endl;
    //cout << "sampleCount: " << sampleCount << endl;
    
    workCArray.resize(static_cast<long unsigned int>(sampleCount));

    for (std::size_t i = 0; i < sampleCount; i++)
    {
	workCArray[i] = samples[i];
	workFArray[i] = static_cast<float>(samples[i]);
    }

    float lowersum = 0;
    float uppersum = 0;
    float lowermax = 0;
    float uppermax = 0;
    int lowermaxfreq = 0;
    int uppermaxfreq = 0;
    for (int i = 0; i < 4; i++)
    {
	freq_mags[i] = goertzel_mag(sampleCount, DTMFFreqs[i], FS, workFArray);
	//cout << DTMFFreqs[i] << "Hz: " << freq_mags[i] << endl;
	lowersum += freq_mags[i];
	if (freq_mags[i] > lowermax)
	{
	    lowermax = freq_mags[i];
	    lowermaxfreq = i;
	}
    }
    for (int i = 4; i < 8; i++)
    {
	freq_mags[i] = goertzel_mag(sampleCount, DTMFFreqs[i], FS, workFArray);
	//cout << DTMFFreqs[i] << "Hz: " << freq_mags[i] << endl;
	uppersum += freq_mags[i];
	if (freq_mags[i] > uppermax)
	{
	    uppermax = freq_mags[i];
	    uppermaxfreq = i;
	}
    }
    
    fft(workCArray);
    applyHammingWindow(workCArray);
    
    //int lower = static_cast<int>(650.0*sampleCount/FS/sampleTime.asSeconds());
    //int upper = static_cast<int>(1650.0*sampleCount/FS/sampleTime.asSeconds());
    // int lower = 600.0*sampleTime.asSeconds();
    int lower = (650.0/(sampleCount/sampleTime.asSeconds()))*sampleCount;
    int upper = (1650.0/(sampleCount/sampleTime.asSeconds()))*sampleCount;
    //cout << "lower index: " << lower << endl;
    //cout << "upper index: " << upper << endl;

    int max = 0;
/*	
	for (Complex c : workCArray)
	if (abs(c) > max)
	max = abs(c);
//*/
///*	
    for (int i = lower; i < upper; i++)
	if (abs(workCArray[i]) > max)
	    max = abs(workCArray[i]);
//*/
	
    //cout << "max = " << max << endl;
    //cout << "highest freq: " << sampleCount/sampleTime.asSeconds() << endl;
    //for (size_t i = 650*sampleTime.asSeconds(); i < sampleCount; i++)
    //if ()
    //threshold = 0.15*max;
    lowerthreshold = lowersum/8;
    upperthreshold = uppersum/4;


    //cout << "thresholds: " << lowerthreshold << " " << upperthreshold << endl;
/*
    peakFreqs.clear();
    for (int i = lower; i < upper; i++)
	//for (size_t i = 0; i < sampleCount / 2; i++) //Division by 2!!! excluding the mirrored freqencies
	if (abs(workCArray[i]) > threshold)
	    peakFreqs.push_back(i / sampleTime.asSeconds());
*/
    //cout << "ffffff" << endl;
    /*
      for(std::size_t i = 0; i < sampleCount; i++)
      if(abs(workCArray[i]) > threshold)
      cout << i/sampleTime.asSeconds() << ", ";
      cout << endl;
    */
    //cout << "ddd" << endl;

/*
    DTMF_byte = 0;

    for (int i = 0; i < 4; i++)
	if(goertzel_mag(sampleCount, DTMFFreqs[i], FS, workFArray) > lowerthreshold)
	    DTMF_byte |= 1 << i;
    
    for (int i = 4; i < 8; i++)
	if(goertzel_mag(sampleCount, DTMFFreqs[i], FS, workFArray) > upperthreshold)
	    DTMF_byte |= 1 << i;
*/
    DTMF_byte = 0;
    DTMF_byte |= (1 << lowermaxfreq) | (1 << uppermaxfreq);

    if ((lowermax < lowersum/2) || (uppermax < uppersum/2))
	DTMF_byte = 0;
    
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
    /*
      array<double, 8> DTMF_tones;
      int max = 0;
      int min = 2147483647; //INT_MAX
    */
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
    /*
      cout << "length of peakFreqs: " << peakFreqs.size() << endl;
      for (double freq : peakFreqs)
      for (int i = 0; i < 8; i++)
      if (abs(freq - DTMFFreqs[i]) < 30)
      DTMF_byte |= 1 << i;
    */
    //cout << "asd " << DTMF_byte << endl;
    cout << "DTMF_byte: " << bitset<8>(DTMF_byte).to_string() << endl;

    switch (DTMF_byte)
    {
    case (1 << 4) + (1 << 0) :
	*currentTone = DTMF_1;
	//cout << "DTMF_1" << endl;
	break;

    case (1 << 5) + (1 << 0) :
	*currentTone = DTMF_2;
	//cout << "DTMF_2" << endl;
	break;

    case (1 << 6) + (1 << 0) :
	*currentTone = DTMF_3;
	//cout << "DTMF_3" << endl;
	break;

    case (1 << 7) + (1 << 0) :
	*currentTone = DTMF_A;
	//cout << "DTMF_A" << endl;
	break;

    case (1 << 4) + (1 << 1) :
	*currentTone = DTMF_4;
	//cout << "DTMF_4" << endl;
	break;

    case (1 << 5) + (1 << 1) :
	*currentTone = DTMF_5;
	//cout << "DTMF_5" << endl;
	break;

    case (1 << 6) + (1 << 1) :
	*currentTone = DTMF_6;
	//cout << "DTMF_6" << endl;
	break;

    case (1 << 7) + (1 << 1) :
	*currentTone = DTMF_B;
	//cout << "DTMF_B" << endl;
	break;

    case (1 << 4) + (1 << 2) :
	*currentTone = DTMF_7;
	//cout << "DTMF_7" << endl;
	break;

    case (1 << 5) + (1 << 2) :
	*currentTone = DTMF_8;
	//cout << "DTMF_8" << endl;
	break;

    case (1 << 6) + (1 << 2) :
	*currentTone = DTMF_9;
	//cout << "DTMF_9" << endl;
	break;

    case (1 << 7) + (1 << 2) :
	*currentTone = DTMF_C;
	//cout << "DTMF_C" << endl;
	break;

    case (1 << 4) + (1 << 3) :
	*currentTone = DTMF_STAR;
	//cout << "DTMF_STAR" << endl;
	break;

    case (1 << 5) + (1 << 3) :
	*currentTone = DTMF_0;
	//cout << "DTMF_0" << endl;
	break;

    case (1 << 6) + (1 << 3) :
	*currentTone = DTMF_HASH;
	//cout << "DTMF_HASH" << endl;
	break;

    case (1 << 7) + (1 << 3) :
	*currentTone = DTMF_D;
	//cout << "DTMF_D" << endl;
	break;

    default:
	*currentTone = DTMF_UNKNOWN;
	cout << "DTMF_UNKNOWN" << endl;
	/*	
		for(double freq : peakFreqs)
		cout << freq << ", ";
		cout << endl;
	*/	
	break;
    }
    //cout << "fff" << endl;
    cout << "DTMF TYPE: " << *currentTone << endl;
}
