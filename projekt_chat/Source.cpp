#include "DLL.hpp"
#include "DTMF.hpp"

#include <iostream>

int main()
{
	DLL myDLL(300);
//	myDLL.read();

///*
	while (true)
	{
		cout << endl;
		cout << "Enter your message:" << endl;
		string userInput;
		cin >> userInput;
		cout << endl;

		myDLL.send(userInput);
	}
// */	

	int x;
	cin >> x;

	return 0;
}
