#include "APP.hpp"
#include "Windows.h"



APP::APP()
{
}

void APP::start()
{
	/*
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions

								//MoveWindow(window_handle, x, y, width, height, redraw_window);
	MoveWindow(console, r.left, r.top, 460, 500, TRUE);

	string msg;
	cout << "==========================================" << endl;
	cout << "=== Welcome to my very cool chat room! ===" << endl;
	cout << "==========================================" << endl << endl << endl;
	*/

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
