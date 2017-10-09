#include "DLL.hpp"
#include "DTMF.hpp"

#include <iostream>

int main()
{
	DLL myDLL("Title");
	myDLL.send("Hello India!");
//	myDLL.playTone();
//	DTMF myTest(250);
//	myTest.play_wait(DTMF_1);

	return 0;
}