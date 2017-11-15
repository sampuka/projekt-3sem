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
	DLL(string);
//	void playTone();
	void send(std::string); //should block
	string interpret(DTMF_type);
	string read(int);
    ~DLL();
    
private:
    DTMF* dtmf;
	string title;
	int packetNumber;
};

#endif
