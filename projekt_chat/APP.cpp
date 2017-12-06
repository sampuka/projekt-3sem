#include "APP.hpp"



APP::APP()
{
}

void APP::start()
{
	DLL myDLL(100);

	myDLL.beginRead();
	cout << "--------------------------------------" << endl;
	cout << endl << "Welcome to \"projekt_chat\", start chatting!" << endl << endl;

	while (true)
	{
		cout << endl;
		//cout << "Enter your message:" << endl;
		string userInput;

		getline(cin, userInput);

		cout << endl;

		myDLL.sendMore(userInput);
	}
}


APP::~APP()
{
}
