#include "DLL.hpp"
#include "DTMF.hpp"

#include <string>
#include <bitset>
#include <iostream>

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
	DTMF dtmf(250);

	// Packet: Header

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
			cout << "Sending security nibble\t11\tDTMF_4" << endl;
			dtmf.play_wait(DTMF_4);
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
				cout << "Sending data nibble\t11\tDTMF_4" << endl;
				dtmf.play_wait(DTMF_4);
			}
		}
	}
}

/*
DTMF::DTMF()
{
    dtmf = DTMF::DTMF(250)
}*/

/*
void DTMF::send(string)
{
    // for x : i osv find ud af det din odder
}
*/

/*
string DTMF::read()
{
    return "ikke implementeret";
}
*/

DLL::~DLL()
{
}

