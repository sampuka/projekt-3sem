#include "DLL.hpp"
#include "DTMF.hpp"
#include "APP.hpp"

#include <iostream>
#include <random>
#include <thread>
#include <mutex>

int main()
{
/*	DLL myDLL(100);
	
	myDLL.beginRead();

	while (true)
	{
		cout << endl;
		cout << "Enter your message:" << endl;
		string userInput;

		getline(cin, userInput);

		cout << endl;

		myDLL.send(userInput);
	}*/
	
	APP myChat;

	myChat.start();

	int x;
	cin >> x;

	return 0;
}
