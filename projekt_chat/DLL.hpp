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
//	void playTone();
	int send(std::string); //should block
	string interpret(DTMF_type);
	void beginRead();
	void debugOutput(string);
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
	vector<string> receivedMessages;
};

#endif
