#include "DLL.hpp"

#include <vector>
#include <string>
#include <bitset>
#include <iostream>
#include <sstream>
#include <thread>

#define DEBUG false

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
	
#define DATA_00 DTMF_9
#define DATA_01 DTMF_C
#define DATA_10 DTMF_HASH
#define DATA_11 DTMF_D
#define DATA_ACK0 DTMF_6
#define DATA_ACK1 DTMF_B
#define DATA_START DTMF_8
#define DATA_STOP DTMF_0

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

// Interpret DTMF_Type
string DLL::interpret(DTMF_type varType)
{
	if (varType == DATA_00)
		return "00";
	else if (varType == DATA_01)
		return "01";
	else if (varType == DATA_10)
		return "10";
	else if (varType == DATA_11)
		return "11";
/*	else if (varType == DTMF_4)
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
		return "??";*/
	else
		return "??";
}

// Send data
int DLL::send(std::string varStr)
{
	int resendCount = 0;

	if (isSending || isReceiving)
	{
		debugOutput("Transmission in progress. Please wait...");
		return 0;
	}

	// Flag: Sending
	isSending = true;

	// Label for reset
send_reset:

	// Send START flag
	debugOutput("Sending flag\t\tSTART");
	dtmf->play_wait(DATA_START);

	// Calculate and send packet number
	int packetNumber = (sentMessages % 2);
	debugOutput("Sending packet number\t0" + to_string(packetNumber));
	switch(packetNumber)
	{
	case 0:
		dtmf->play_wait(DATA_00);
		break;
	case 1:
		dtmf->play_wait(DATA_01);
		break;
	}
	sentMessages++;

	// Send data two bits at a time (can be made better?)
	for (std::size_t i = 0; i < varStr.size(); i++)
	{
		for (int indeks = 0; indeks < 8; indeks += 2)
		{
			
			string caseVar = (bitset<8>(varStr[i]).to_string()).substr(indeks, 2);
			debugOutput("Sending data nibble\t" + caseVar);
			if (caseVar == "00")
			{
				dtmf->play_wait(DATA_00);
			}
			else if (caseVar == "01")
			{
				dtmf->play_wait(DATA_01);
			}
			else if (caseVar == "10")
			{
				dtmf->play_wait(DATA_10);
			}
			else if (caseVar == "11")
			{
				dtmf->play_wait(DATA_11);
			}
		}
	}

	// Calculate and send security bits
	int checksum = 0;
	for (int i = 0; i < varStr.size(); i++)
	{
		checksum += varStr[i];
	}

	while (checksum > 255)
	{
		int lsb = checksum & 0b11111111;
		int msb = checksum >> 8;
		checksum = msb + lsb;
	}


	for (int indeks = 0; indeks < 8; indeks += 2)
	{
		string switchSecure = (bitset<8>(checksum).to_string()).substr(indeks, 2);
		debugOutput("Sending security nibble\t" + switchSecure);
		if (switchSecure == "00")
		{
			dtmf->play_wait(DATA_00);
		}
		else if (switchSecure == "01")
		{
			dtmf->play_wait(DATA_01);
		}
		else if (switchSecure == "10")
		{
			dtmf->play_wait(DATA_10);
		}
		else if (switchSecure == "11")
		{
			dtmf->play_wait(DATA_11);
		}
	}

	// Send STOP flag
	debugOutput("Sending flag\t\tSTOP\n");
	dtmf->play_wait(DATA_STOP);

	// Listen for acknowledge
	for (int i = 0; i <= 200; i++)
	{
		if (((dtmf->listen() == DATA_ACK0) && (packetNumber == 1)) || ((dtmf->listen() == DATA_ACK1) && (packetNumber == 0)))
		{
			debugOutput("Message acknowledged by receiver.");
			isSending = false;
			return 0;
		}
		mysleep(10);
	}

	// Check for reset maximum
	if (resendCount > 2)
	{
		debugOutput("Resend maximum reached. Message not delivered.");
		return 0;
	}

	// Reset if none is received
	debugOutput("Message not acknowledged, resend attempt " + to_string(resendCount+1) + "...\n");
	sentMessages--;
	resendCount++;
	goto send_reset;
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

	// Clear variables upon start/reset
	received_msg = "";				
	number_str = "";
	data_str = "";				
	checksum_str = "";
	received_data.clear();

	// Stuck in loop; wait for start
	while ((dtmf->listen() != DATA_START) || isSending)
	{
		//cout << interpret(dtmf->listen()) << endl;
		// Busy waiting...
	}

	debugOutput("Hearing flag\tSTART\t(1/2)");
	
	// Wait half a tone, check if tone is still START flag
	mysleep(time/2);
	if (dtmf->listen() != DATA_START)
	{
		goto read_reset;
	}

	// Receive started; set flag
	isReceiving = true;
	debugOutput("Hearing flag\tSTART\t(2/2)");

	// Wait until middle of first tone
	//mysleep(time);
	mysleep(0.75*time);

	// Start recording to received_data

	debugOutput("Recording started.");

	while (dtmf->listen() != DATA_STOP)	// Record while flag is not STOP
	{	
		DTMF_type sample[3] ;
		sample[0] = dtmf->listen();
		mysleep(0.25*time);

		sample[1] = dtmf->listen();
		mysleep(0.25*time);

		sample[2] = dtmf->listen();

		// Tale three listen samples
		if ((sample[0] == sample[1]) && (sample[0] != DTMF_UNKNOWN))
		{
			received_data.push_back(sample[0]);
		}
		else if ((sample[0] == sample[2]) && (sample[0] != DTMF_UNKNOWN))
		{
			received_data.push_back(sample[0]);
		}
		else if ((sample[1] == sample[2]) && (sample[1] != DTMF_UNKNOWN))
		{
			received_data.push_back(sample[1]);
		}
		else if ((sample[0] == DTMF_UNKNOWN) && (sample[1] == DTMF_UNKNOWN) && (sample[2] == DTMF_UNKNOWN))
		{
			received_data.push_back(DTMF_UNKNOWN);
		}
		else for (int i = 0; i < 3; i++)
		{
			if (sample[i] != DTMF_UNKNOWN)
			{
				received_data.push_back(sample[i]);
				break;
			}
		}

		debugOutput("Hearing\t" + interpret(received_data[(received_data.size()) - 1]));
		mysleep(0.5*time);

		/*received_data.push_back(dtmf->listen());
		cout << "Hearing\t" << interpret(received_data[(received_data.size())-1]) << endl;
		mysleep(time);*/
	}
	
	debugOutput("Hearing flag\tSTOP");
	debugOutput("Recording ended.\n");

	// Interpret numbering
	for (int i = 0; i < 1; i++)
	{
		number_str += interpret(received_data[i]);
	}
	
	// Interpret data message
	for (unsigned int i = 1; i < (received_data.size() - 4); i++)
	{
		data_str += interpret(received_data[i]);
	}
	
	// Interpret checksum 
	for (unsigned int i = (received_data.size() - 4); i < received_data.size(); i++)
	{
		checksum_str += interpret(received_data[i]);
	}

	// Output
	debugOutput("Received number:\t" + number_str);
	debugOutput("Received checksum:\t" + checksum_str + '\n');

	stringstream sstream(data_str);

	while (sstream.good())
	{
		bitset<8> bits;
		sstream >> bits;
		received_msg += char(bits.to_ulong());
	}
	
	// Check for checksum match, reset if negative
	int rcv_checksum = 0;
	for (int i = 0; i < received_msg.size(); i++)
	{
		rcv_checksum += received_msg[i];
	}

	while (rcv_checksum > 255)
	{
		int lsb = rcv_checksum & 0b11111111;
		int msb = rcv_checksum >> 8;
		rcv_checksum = msb + lsb;
	}

	debugOutput("Calculated checksum:\t" + (bitset<8>(rcv_checksum).to_string()) + '\n');
	
	if ((bitset<8>(rcv_checksum).to_string()) != checksum_str)
	{
		debugOutput("Unmatching checksums, message discarded. Starting over...");
		goto read_reset;
	}
	
	debugOutput("Matching checksums.\n");
	
	// Check for numbering, save message in receivedMessages.
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

	debugOutput("Sending acknowledge\tACK" + to_string(ackNumber));

	switch (ackNumber)
	{
	case 0:
		dtmf->play_wait(DATA_ACK0);
		dtmf->play_wait(DATA_ACK0);
		dtmf->play_wait(DATA_ACK0);
		break;
	case 1:
		dtmf->play_wait(DATA_ACK1);
		dtmf->play_wait(DATA_ACK1);
		dtmf->play_wait(DATA_ACK1);
		break;
	}
	sentAcks++;

	// Clear receiving flag
	isReceiving = false;

	// Return to start
	goto read_reset;
}

// Begin read() as its own thread
void DLL::beginRead()
{
	read_thread = new thread(&DLL::read, this);
}

void DLL::debugOutput(string _output)
{
	if (DEBUG)
	{
		cout << _output << endl;
	}
}

DLL::~DLL()
{
}

