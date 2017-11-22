#ifndef DLL_H
#define DLL_H

#include "DTMF.hpp"
#include <string>
#include <vector>
#include <iostream>


using namespace std;

class DLL
{
public:
    DLL();
	DLL(int);
//	void playTone();
	int send(std::string); //should block
	string interpret(DTMF_type);
	void read();
    ~DLL();
    
private:
    DTMF* dtmf;
	string title;
	int time;
	int packetNumber;
	bool isReceiving;
	bool isSending;
	vector<string> receivedMessages;
};

#endif
