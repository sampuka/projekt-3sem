#ifndef DLL_H
#define DLL_H

#include <string>
#include "DTMF.hpp"

class DLL
{
public:
    DLL();
    void send(std::string); //should block
    std::string read();
    ~DLL();
    
private:
    DTMF dtmf;
};

#endif
