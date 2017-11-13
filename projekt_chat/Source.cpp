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

	DTMF dtmf(500);
	dtmf.startRecording();
	
	//dtmf.play_wait(DTMF_1);

	int x;
	cin >> x;

	return 0;
}