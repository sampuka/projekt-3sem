#ifndef DLL_H
#define DLL_H

#include "DTMF.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <thread>

using namespace std;

class DLL
{
public:
    DLL();
	DLL(int);
	int send(std::string);			// Send individual message
	string interpret(DTMF_type);	// Interpret DTMF_type
	void beginRead();				// Intitialize read() in thread
	void debugOutput(string);		// Togglable output
	string getMsg();				// Used for pop_front (not currently in use)
	void sendMore(string);			// Sending more messages, with message seperator
    ~DLL();
    
private:
	void read();
	thread* read_thread;
    DTMF* dtmf;
	string title;
	int time;
	int sentMessages;
	int sentAcks;
	bool isReceiving;
	bool isSending;
	vector<string> msg_received;
};

#endif
