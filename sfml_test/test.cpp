#include <iostream>
#include "DTMF.hpp"

using namespace std;

int main()
{
    DTMF dtmf(500);

    dtmf.play_wait(DTMF_1);

    dtmf.play_wait(DTMF_2);

    dtmf.play_wait(DTMF_3);

    dtmf.play_wait(DTMF_STAR);

    return 0;
}
