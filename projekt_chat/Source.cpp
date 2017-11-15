#include "DLL.hpp"
#include "DTMF.hpp"

#include <iostream>

int main()
{


	DLL myDLL("Hello india");
	myDLL.read(1000);


//	cout << myDLL.interpret(DTMF_A) << endl;

//	myDLL.send("Hello india");

//	dtmf.play_wait(DTMF_1);



	int x;
	cin >> x;

	return 0;
}