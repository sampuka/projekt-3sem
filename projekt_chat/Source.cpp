#include "DLL.hpp"
#include "DTMF.hpp"

#include <iostream>

int main()
{


	DLL myDLL("Hello india");
//	myDLL.read(1000);

	while (true)
	{
		cout << endl;
		cout << "Enter your message:" << endl;
		string userInput;
		cin >> userInput;
		cout << endl;

		myDLL.send(userInput);
	}

	int x;
	cin >> x;

	return 0;
}