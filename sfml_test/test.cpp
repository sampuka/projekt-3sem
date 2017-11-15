#include <iostream>
#include <vector>
#include "unistd.h"
#include "DTMF.hpp"

using namespace std;

int main()
{
    DTMF dtmf(250);
    dtmf.startRecording();

    vector<DTMF_type> tlfNummer = {DTMF_3, DTMF_2, DTMF_7, DTMF_8, DTMF_4, DTMF_2, DTMF_1, DTMF_8};
    //dtmf.play_list(tlfNummer);

    while(1)
    {
	nanosleep((const struct timespec[]){{0, 50000000L}}, NULL);
	cout << "tone: " << dtmf.listen() << endl;
    }
    
    int x;
    cin >> x;
    
/*
    dtmf.play_wait(DTMF_1);

    dtmf.play_wait(DTMF_2);

    dtmf.play_wait(DTMF_3);

    dtmf.play_wait(DTMF_STAR);
*/
    
    return 0;
}
