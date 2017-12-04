#include "APP.hpp"



APP::APP()
{
	cout << "Welcome to \"projekt_chat\", start chatting!" << endl << endl;
}

void APP::start()
{
	DLL myDLL(100);

	myDLL.beginRead();

	while (true)
	{
		cout << endl;
		cout << "Enter your message:" << endl;
		string userInput;

		getline(cin, userInput);

		cout << endl;

		myDLL.send(userInput);
	}
}


APP::~APP()
{
}
