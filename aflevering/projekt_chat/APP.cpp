#include "APP.hpp"



APP::APP()
{
}

void APP::start()
{
	DLL myDLL(100);

	myDLL.beginRead();
	cout << "--------------------------------------" << endl;

	string userName;
	cout << "What is your display name?" << endl;
	getline(cin, userName);
	cout << endl;

	cout << "Welcome to \"projekt_chat\", " << userName << "! - Start chatting!" << endl << endl;

	while (true)
	{
		string userInput;

		getline(cin, userInput);

		if (userInput.length() > 0)
		{
			myDLL.sendMore((userName + ": " + userInput), 3);
		}

	}
}

APP::~APP()
{
}
