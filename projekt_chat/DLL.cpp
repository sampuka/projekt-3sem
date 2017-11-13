#include "DLL.hpp"
#include "DTMF.hpp"
#include <vector>
#include <string>
#include <bitset>
#include <iostream>
#include <windows.h>
#include <sstream>

using namespace std;

DLL::DLL()
{
	title = "";
}

DLL::DLL(string varTitle)
{
	title = varTitle;
	packetNumber = 0;
}

// Send data
void DLL::send(std::string varStr)
{
	DTMF dtmf(200);

	// Packet: Header

	// Flag: Start
	cout << "Sending flag\t\tSTART\tDTMF_4" << endl;
	dtmf.play_wait(DTMF_4);

	// Packet number
	switch(packetNumber)
	{
	case 0:
		packetNumber = 1;
		cout << "Sending packet number\t00\tDTMF_2" << endl;
		dtmf.play_wait(DTMF_1);
		break;
	case 1:
		packetNumber = 0;
		cout << "Sending packet number\t01\tDTMF_2" << endl;
		dtmf.play_wait(DTMF_2);
		break;
	}
	
	// Packet secure
	int varSecure = 0;
	for (std::size_t i = 0; i < varStr.size(); i++)
	{
		varSecure += varStr[i];
	}
	for (int indeks = 0; indeks <= 6; indeks += 2)
	{
		string switchSecure = (bitset<6>(varSecure).to_string()).substr(indeks, 2);
		if (switchSecure == "00")
		{
			cout << "Sending security nibble\t00\tDTMF_1" << endl;
			dtmf.play_wait(DTMF_1);
		}
		else if (switchSecure == "01")
		{
			cout << "Sending security nibble\t01\tDTMF_2" << endl;
			dtmf.play_wait(DTMF_2);
		}
		else if (switchSecure == "10")
		{
			cout << "Sending security nibble\t10\tDTMF_3" << endl;
			dtmf.play_wait(DTMF_3);
		}
		else if (switchSecure == "11")
		{
			cout << "Sending security nibble\t11\tDTMF_A" << endl;
			dtmf.play_wait(DTMF_A);
		}
	}

	// Packet: Data
	for (std::size_t i = 0; i < varStr.size(); i++)
	{
		for (int indeks = 0; indeks <= 8; indeks += 2)
		{
			string caseVar = (bitset<8>(varStr[i]).to_string()).substr(indeks, 2);

			if (caseVar == "00")
			{
				cout << "Sending data nibble\t00\tDTMF_1" << endl;
				dtmf.play_wait(DTMF_1);
			}
			else if (caseVar == "01")
			{
				cout << "Sending data nibble\t01\tDTMF_2" << endl;
				dtmf.play_wait(DTMF_2);
			}
			else if (caseVar == "10")
			{
				cout << "Sending data nibble\t10\tDTMF_3" << endl;
				dtmf.play_wait(DTMF_3);
			}
			else if (caseVar == "11")
			{
				cout << "Sending data nibble\t11\tDTMF_A" << endl;
				dtmf.play_wait(DTMF_A);
			}
		}
	}

	// Flag: Stop
	cout << "Sending flag\t\tSTOP\tDTMF_4" << endl;
	dtmf.play_wait(DTMF_4);
}

// Read data

string DLL::read()
{
/*	int dtmf_sample = 0;				// Delete later - current sample sound
	int check = 0;						// Acknowledgement check init
	string received_msg;				// Final received message, translated 
	string temp_str = "";				// Temprorary string of recieved bits, without security bits
	vector<dtmf_type> received_data;	// Vector storing received bits
	
	while (check == 0)
	{
		if (dtmf_sample() == dtmf_flag)
		{
			Sleep(50);
			if (dtmf_sample() == dtmf_flag)
			{
				check = 1;
			}
		}
	}

	Sleep(100);

	while (dtmf_sample() != dtmf_flag)
	{
		received_data.push_back(dtmf_sample());
		Sleep(100);
	}
*/
	return "Not implemented";
}

DLL::~DLL()
{
}

