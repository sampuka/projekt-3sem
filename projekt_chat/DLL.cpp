#include "DLL.hpp"

#include <vector>
#include <string>
#include <bitset>
#include <iostream>
#include <sstream>
#include <thread>

#define DEBUG true

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
#define DATA_SEP DTMF_3

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
	// Check for case: empty string
	if (varStr == "")
	{
		debugOutput("Invalid message input");
		return 0;
	}

	// Reset resend maximum
	int resendCount = 0;

	// Check for sending/receiving flags
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
/*	if (resendCount > 2)
	{
		debugOutput("Resend maximum reached. Message not delivered.");
		sentMessages--;
		isSending = false;
		return 0;
	}*/

	// Reset if none is received
	debugOutput("Message not acknowledged, resend attempt " + to_string(resendCount+1) + "...\n");
	sentMessages--;
	resendCount++;
	goto send_reset;
}

// Read data
void DLL::read()
{
	string msg_partition;				// Received message for individual packet.
	string msg_assemble;				// String used for stitching together packets

	int ackNumber;						// Acknowledge counter
	vector<DTMF_type> received_data;	// Vector storing received bits

	string number_str;					// Temprorary string of recieved numbering
	string data_str;					// Temprorary string of recieved data
	string checksum_str;				// Temprorary string of recieved checksum

	// Location for reset
read_reset:								

	// Clear variables upon start/reset
	msg_partition = "";		
	number_str = "";
	data_str = "";				
	checksum_str = "";
	received_data.clear();

	// Stuck in loop; wait for start
	while ((dtmf->listen() != DATA_START) || isSending)
	{
		if (dtmf->listen() == DATA_SEP)
		{
			if (msg_assemble.length() > 0)
			cout << msg_assemble << endl;
			//msg_received.push_back(msg_assemble);
			msg_assemble = "";
		}
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

	// Wait until 0.25 of first tone (first sample)
	mysleep(0.75*time);

	// Start recording to received_data

	debugOutput("Recording started.");

	/*
	vector<DTMF_type> dtmf_samples;

	while (dtmf->listen() != DATA_STOP)	// Record while flag is not STOP
	{
		// Sample 3 tones
		dtmf_samples.push_back(dtmf->listen());
		mysleep(0.25*time);

		dtmf_samples.push_back(dtmf->listen());
		mysleep(0.25*time);

		dtmf_samples.push_back(dtmf->listen());
		mysleep(0.5*time);
	}

	debugOutput("Hearing flag\tSTOP");
	debugOutput("Recording ended.\n");

	for (int i = 0; i < size(dtmf_samples); i = i + 3)
	{
		// Check for majority / not unknown
		if ((dtmf_samples[i] == dtmf_samples[i+1]) && (i != DTMF_UNKNOWN))
		{
			received_data.push_back(dtmf_samples[i]);
		}
		else if ((dtmf_samples[i] == dtmf_samples[i+2]) && (dtmf_samples[i] != DTMF_UNKNOWN))
		{
			received_data.push_back(dtmf_samples[i]);
		}
		else if ((dtmf_samples[i+1] == dtmf_samples[i+2]) && (dtmf_samples[i+1] != DTMF_UNKNOWN))
		{
			received_data.push_back(dtmf_samples[i+1]);
		}
		else if ((dtmf_samples[i] == DTMF_UNKNOWN) && (dtmf_samples[i+1] == DTMF_UNKNOWN) && (dtmf_samples[i+2] == DTMF_UNKNOWN))
		{
			received_data.push_back(DTMF_UNKNOWN);
		}
		else for (int j = i; j < (i+3); j++)
		{
			if (dtmf_samples[j] != DTMF_UNKNOWN)
			{
				received_data.push_back(dtmf_samples[j]);
				break;
			}
		}
	}
//	*/

	// Old method, maybe reimplement if stuff doesnt work
//	/*
	while (dtmf->listen() != DATA_STOP)	// Record while flag is not STOP
	{	
		// Sample 3 tones
		DTMF_type sample[3];

		sample[0] = dtmf->listen();
		mysleep(0.25*time);

		sample[1] = dtmf->listen();
		mysleep(0.25*time);

		sample[2] = dtmf->listen();

		// Check for majority / not unknown
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
	}
	

	debugOutput("Hearing flag\tSTOP");
	debugOutput("Recording ended.\n");

//	*/


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

	// Debug output received numbering/checksum
	debugOutput("Received number:\t" + number_str);
	debugOutput("Received checksum:\t" + checksum_str + '\n');

	// Recreate message from data
	stringstream sstream(data_str);

	while (sstream.good())
	{
		bitset<8> bits;
		sstream >> bits;
		msg_partition += char(bits.to_ulong());
	}
	
	// Check for checksum match, reset if negative
	int rcv_checksum = 0;
	for (int i = 0; i < msg_partition.size(); i++)
	{
		rcv_checksum += msg_partition[i];
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
		isReceiving = false; // Creates unwanted window - negligibly short.
		goto read_reset;
	}
	
	debugOutput("Matching checksums.\n");
	
	// Check for numbering, if true; add message to rest of message.
	ackNumber = (sentAcks % 2);
	if (stoi(number_str) != ackNumber)
	{
		msg_assemble += (msg_partition + '\b');
	}
	else 
	{
		sentAcks--;
	}

	// Wait and send acknowledge with numbering
	mysleep(time);
	ackNumber = (sentAcks % 2); 

	debugOutput("Sending acknowledge\tACK" + to_string(ackNumber));

	switch (ackNumber)
	{
	case 0:
		dtmf->play_wait(DATA_ACK0);
		break;
	case 1:
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

// Function for enabling/disabling output - see top #define DEBUG
void DLL::debugOutput(string _output)
{
	if (DEBUG)
	{
		cout << _output << endl;
	}
}

// Not currently in use (but nice to have in case of changes)
string DLL::getMsg()
{
	if (size(msg_received) == 0)
	{
		return "";
	}
	string msg = msg_received[0];
	msg_received.erase(msg_received.begin());
	return msg;
}

void DLL::sendMore(string _message, int _partition)
{
	if (_message != "")
	{
		vector<string> partMessage;
		int lastIndex = 0;
		int index = 0;

		while (index < _message.length())
		{
			for (int i = 0; i < _partition; i++)
			{
				if (index < _message.length())
				{
					index++;
				}
			}
			mysleep(time);
			send(_message.substr(lastIndex, (index - lastIndex)));
			lastIndex = index;
		}
		dtmf->play_wait(DATA_SEP);
		debugOutput("Sending data seperator");
	}

}

DLL::~DLL()
{
}

