#include "DLL.hpp"
#include "DTMF.hpp"
#include <vector>
#include <string>
#include <bitset>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include "windows.h"

void mysleep(int ms)
{
    Sleep(ms);
}

#else
#include "unistd.h"

void mysleep(int ms)
{
    const struct timespec temp = {0, ms*1000000};
    nanosleep(&temp, NULL);
}

#endif
	
using namespace std;

DLL::DLL()
{
}

DLL::DLL(int varTime)
{
	// Establish DTMF instance , tone-length = varTime
	dtmf = new DTMF(varTime);		
	dtmf->startRecording();			
	
	// Establish variable 'time' for later use
	time = varTime;					

	// Counter for sent messages and acknowledges
	sentMessages = 0;				
	sentAcks = 1;					

	// Initiate flags for sending and receiving
	isReceiving = false;			
	isSending = false;				
}

// Send data
int DLL::send(std::string varStr)
{
	// Flag: Sending
	isSending = true;

	// Label for reset
send_reset:

	// Send START flag: DTMF_4
	cout << "Sending flag\t\tSTART\tDTMF_4" << endl;
	dtmf->play_wait(DTMF_4);

	// Calculate and send packet number
	int packetNumber = (sentMessages % 2);
	switch(packetNumber)
	{
	case 0:
		cout << "Sending packet number\t00\tDTMF_2" << endl;
		dtmf->play_wait(DTMF_1);
		break;
	case 1:
		cout << "Sending packet number\t01\tDTMF_2" << endl;
		dtmf->play_wait(DTMF_2);
		break;
	}
	sentMessages++;

	// Send data two bits at a time
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

	// Calculate and send security bits
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

	// Send STOP flag: DTMF_D
	cout << "Sending flag\t\tSTOP\tDTMF_D" << endl;
	dtmf->play_wait(DTMF_D);
	cout << endl;

	// Listen for acknowledge
	for (int i = 0; i <= 200; i++)
	{
		if (((dtmf->listen() == DTMF_5) && (packetNumber == 1)) || ((dtmf->listen() == DTMF_6) && (packetNumber == 0)))
		{
			cout << "Message acknowledged by receiver." << endl;
			isSending = false;
			return 0;
		}
		mysleep(10);
	}

	// Reset if none is received
	cout << "Message not acknowledged, resending..." << endl << endl;
	sentMessages--;
	goto send_reset;
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
void DLL::read()
{
	string received_msg;				// Final received message, translated
	string number_str;
	string data_str;					// Temprorary string of recieved bits, without security bits
	string checksum_str;
	vector<DTMF_type> received_data;	// Vector storing received bits
	int ackNumber;
	
read_reset:								// Location for reset

	received_msg = "";				
	number_str = "";
	data_str = "";				
	checksum_str = "";
	received_data.clear();

	// Stuck in loop; wait for start
	while (dtmf->listen()!= DTMF_4)		// Flag = DTMF_4
	{
		//cout << "Hearing\t" << interpret(dtmf->listen()) << endl;
		// Waiting...
	}
	cout << "Hearing flag\tSTART\tDTMF_4\t (1/2)" << endl;
	
	// Wait half a tone, check if tone is still START flag
	mysleep(time/2);
	if (dtmf->listen() != DTMF_4)
	{
		goto read_reset;
	}

	// Receive started; set flag
	isReceiving = true;
	cout << "Hearing flag\tSTART\tDTMF_4\t (2/2)" << endl;

	// Wait until middle of first tone
	mysleep(time);

	// Start recording to received_data
	cout << "Recording started." << endl;
	while (dtmf->listen() != DTMF_D)	// Record while flag is not STOP
	{
		received_data.push_back(dtmf->listen());
		cout << "Hearing\t" << interpret(received_data[(received_data.size())-1]) << endl;
		mysleep(time);
	}
	cout << "Hearing flag\tSTOP\tDTMF_4" << endl;
	cout << "Recording ended." << endl;

	// Convert message to characters -> string
	for (int i = 0; i < 1; i++)
	{
		number_str += interpret(received_data[i]);
	}
	
	for (unsigned int i = 1; i < (received_data.size() - 3); i++)
	{
		data_str += interpret(received_data[i]);
	}
	
	for (unsigned int i = (received_data.size() - 3); i < received_data.size(); i++)
	{
		checksum_str += interpret(received_data[i]);
	}

	cout << endl;
	cout << "Received number:\t" << number_str << endl << endl;
	cout << "Received checksum:\t" << checksum_str << endl;

	stringstream sstream(data_str);

	while (sstream.good())
	{
		std::bitset<8> bits;
		sstream >> bits;
		received_msg += char(bits.to_ulong());
	}
	
	// Check for checksum match, reset if negative
	int rcv_checksum = 0;
	for (std::size_t i = 0; i < received_msg.size(); i++)
	{
		rcv_checksum += received_msg[i];
	}

	cout << "Calculated checksum:\t" << (bitset<6>(rcv_checksum).to_string()) << endl;
	cout << endl;

	if ((bitset<6>(rcv_checksum).to_string()) != checksum_str)
	{
		cout << "Unmatching checksums, message discarded. Starting over..." << endl;
		//mysleep(500);
		goto read_reset;
	}
	cout << "Matching checksums." << endl << endl;
	
	// Check for numbering, gem besked i reveivedMessages.
	ackNumber = (sentAcks % 2);
	if (stoi(number_str) != ackNumber)
	{
		cout << "Received message:\t" << received_msg << endl;	// Output message
		receivedMessages.push_back(received_msg);				// Add received message to message buffer
	}
	else 
	{
		sentAcks--;
	}

	// Wait and send acknowledge with numbering
	mysleep(100);
	ackNumber = (sentAcks % 2); 

	switch (ackNumber)
	{
	case 0:
		cout << "Sending acknowledge\tACK0" << endl;
		dtmf->play_wait(DTMF_5); 
		dtmf->play_wait(DTMF_5);
		dtmf->play_wait(DTMF_5);
		break;
	case 1:
		cout << "Sending acknowledge\tACK1" << endl;
		dtmf->play_wait(DTMF_6); 
		dtmf->play_wait(DTMF_6);
		dtmf->play_wait(DTMF_6);
		break;
	}
	sentAcks++;

	// Clear receiving flag
	isReceiving = false;

	// Return to start
	goto read_reset;
}

DLL::~DLL()
{
}

