#include "DLL.hpp"
#include "DTMF.hpp"

#include <iostream>

int main()
{
	/*
	DLL myDLL("Hello india");
	myDLL.send("Hello india");
	int var;
	*/ 

	DTMF dtmf(10000);
	dtmf.play_wait(DTMF_1);

	return 0;
}