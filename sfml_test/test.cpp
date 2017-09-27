#include <iostream>
#include "DTMF.hpp"

using namespace std;

int main()
{
    DTMF dtmf(500);

    dtmf.play_wait(DTMF_A);

    dtmf.play_wait(DTMF_B);

    dtmf.play_wait(DTMF_C);

    return 0;
}
