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
	dtmf = new DTMF(200);
	dtmf->startRecording();
	title = varTitle;
	packetNumber = 0;
}

// Send data
void DLL::send(std::string varStr)
{

	// Packet: Header

	// Flag: Start
	cout << "Sending flag\t\tSTART\tDTMF_4" << endl;
	dtmf->play_wait(DTMF_4);

	// Packet number
	switch(packetNumber)
	{
	case 0:
		packetNumber = 1;
		cout << "Sending packet number\t00\tDTMF_2" << endl;
		dtmf->play_wait(DTMF_1);
		break;
	case 1:
		packetNumber = 0;
		cout << "Sending packet number\t01\tDTMF_2" << endl;
		dtmf->play_wait(DTMF_2);
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
			dtmf->play_wait(DTMF_1);
		}
		else if (switchSecure == "01")
		{
			cout << "Sending security nibble\t01\tDTMF_2" << endl;
			dtmf->play_wait(DTMF_2);
		}
		else if (switchSecure == "10")
		{
			cout << "Sending security nibble\t10\tDTMF_3" << endl;
			dtmf->play_wait(DTMF_3);
		}
		else if (switchSecure == "11")
		{
			cout << "Sending security nibble\t11\tDTMF_A" << endl;
			dtmf->play_wait(DTMF_A);
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
				dtmf->play_wait(DTMF_1);
			}
			else if (caseVar == "01")
			{
				cout << "Sending data nibble\t01\tDTMF_2" << endl;
				dtmf->play_wait(DTMF_2);
			}
			else if (caseVar == "10")
			{
				cout << "Sending data nibble\t10\tDTMF_3" << endl;
				dtmf->play_wait(DTMF_3);
			}
			else if (caseVar == "11")
			{
				cout << "Sending data nibble\t11\tDTMF_A" << endl;
				dtmf->play_wait(DTMF_A);
			}
		}
	}

	// Flag: Stop
	cout << "Sending flag\t\tSTOP\tDTMF_4" << endl;
	dtmf->play_wait(DTMF_4);
}

// Interpret DTMF_Type
string DLL::interpret(DTMF_type varType)
{
	if (varType == DTMF_1)
		return "00";
	else if (varType == DTMF_2)
		return "01";
	else if (varType == DTMF_3)
		return "10";
	else if (varType == DTMF_A)
		return "11";
	else if (varType == DTMF_4)
		return "44";
	else if (varType == DTMF_5)
		return "55";
	else if (varType == DTMF_6)
		return "66";
	else if (varType == DTMF_B)
		return "BB";
	else if (varType == DTMF_7)
		return "77";
	else if (varType == DTMF_8)
		return "88";
	else if (varType == DTMF_9)
		return "99";
	else if (varType == DTMF_C)
		return "CC";
	else if (varType == DTMF_STAR)
		return "**";
	else if (varType == DTMF_0)
		return "00";
	else if (varType == DTMF_HASH)
		return "##";
	else if (varType == DTMF_D)
		return "DD";
	else if (varType == DTMF_UNKNOWN)
		return "??";
	else
		return "??";
}

// Read data
string DLL::read(int i)
{
	vector<DTMF_type> received_data;	// Vector storing received bits

	// Wait for start flag
	reset:
	while (dtmf->listen()!= DTMF_4) // Flag = DTMF_4
	{
	//	cout << "Hearing\t" << interpret(dtmf->listen()) << endl;
		Sleep(100);
	}

	cout << "Hearing flag\tSTART\tDTMF_4\t (1/2)" << endl;
	Sleep(i/2);

	if (dtmf->listen() != DTMF_4)
	{
		goto reset;
	}

	cout << "Hearing flag\tSTART\tDTMF_4\t (2/2)" << endl;

	// Wait until middle of first tone
	Sleep(i);

	// Start recording
	cout << "Starting recording..." << endl;

	while (dtmf->listen() != DTMF_4)
	{
		received_data.push_back(dtmf->listen());
		cout << "Hearing\t" << interpret(received_data[(received_data.size())-1]) << endl;
		Sleep(i);
	}
	cout << "Ended recording..." << endl;

	for (i = 0; i < received_data.size(); i++)
	{
		cout << interpret(received_data[i]) << endl;
	}

	return "Not implemented";
}

DLL::~DLL()
{
}

