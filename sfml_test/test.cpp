#include <iostream>
#include <vector>
#include "DTMF.hpp"

using namespace std;

int main()
{
    DTMF dtmf(250);
    dtmf.start_recording();

    vector<DTMF_type> tlfNummer = {DTMF_3, DTMF_2, DTMF_7, DTMF_8, DTMF_4, DTMF_2, DTMF_1, DTMF_8};
    dtmf.play_list(tlfNummer);

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
