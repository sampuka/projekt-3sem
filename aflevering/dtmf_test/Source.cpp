#include "DTMF.hpp"

#include <iostream>
#include <vector>
#include <random>

using namespace std;

#ifdef _WIN32
#include "windows.h"

void mysleep(int ms)
{
    Sleep(ms);
}

#else
#include "unistd.h"

void mysleep(int ms)
{
    const struct timespec temp = {0, ms*1000000};
    nanosleep(&temp, NULL);
}

#endif

#define INTERVAL 100
#define COUNT 10

int main()
{
    DTMF dtmf(INTERVAL*2);

    vector<DTMF_type> tones = {DTMF_1, DTMF_2, DTMF_3, DTMF_A, DTMF_4, DTMF_5, DTMF_6, DTMF_B, DTMF_7, DTMF_8, DTMF_9, DTMF_C, DTMF_STAR, DTMF_0, DTMF_HASH, DTMF_D};

    int T = COUNT;
    
///*
    DTMF_type tone = DTMF_UNKNOWN;
    while(T--)
    {
	tone = tones[rand() % tones.size()];
	cout << tone << endl;
        dtmf.play_wait(tone);
    }
//*/
///*
    dtmf.startRecording();
    DTMF_type a;
    DTMF_type b;
    while (1)
    {
	a = dtmf.listen();
        mysleep(INTERVAL);
	
	b = dtmf.listen();
        mysleep(INTERVAL);
	
	if (a == b)
	    cout << a << endl;
	else if (a == DTMF_UNKNOWN)
	    cout << b << endl;
	else if (b == DTMF_UNKNOWN)
	    cout << a << endl;
	else
	    cout << DTMF_UNKNOWN << endl;
    }
//*/
    return 0;
}
