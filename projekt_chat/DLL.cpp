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
}

// Send string
void DLL::send(std::string varStr)
{
	DTMF dtmf(250);
	for (std::size_t i = 0; i < varStr.size(); ++i)
	{
		for (int indeks = 0; indeks <= 8; indeks = indeks + 2)
		{
			string caseVar = (bitset<8>(varStr[i]).to_string()).substr(indeks, 2);

			if (caseVar == "00")
			{
				cout << "Sending 00..." << endl;
				dtmf.play_wait(DTMF_1);
			}
			else if (caseVar == "01")
			{
				cout << "Sending 01..." << endl;
				dtmf.play_wait(DTMF_2);
			}
			else if (caseVar == "10")
			{
				cout << "Sending 10..." << endl;
				dtmf.play_wait(DTMF_3);
			}
			else if (caseVar == "11")
			{
				cout << "Sending 11..." << endl;
				dtmf.play_wait(DTMF_4);
			}
			//cout << (bitset<8>(varStr[i]).to_string()).substr(indeks, 2);
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

