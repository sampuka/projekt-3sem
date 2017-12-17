#include "DTMF_type.hpp"

#include <iostream>
#include <string>

std::ostream& operator<< (std::ostream& os, const DTMF_type& dtmf)
{
    std::string to_cout = "DTMF_";
    switch(dtmf)
    {
    case DTMF_1:
	to_cout +="1";
	break;
    case DTMF_2:
	to_cout +="2";
	break;
    case DTMF_3:
	to_cout +="3";
	break;
    case DTMF_A:
	to_cout +="A";
	break;
    case DTMF_4:
	to_cout +="4";
	break;
    case DTMF_5:
	to_cout +="5";
	break;
    case DTMF_6:
	to_cout +="6";
	break;
    case DTMF_B:
	to_cout +="B";
	break;
    case DTMF_7:
	to_cout +="7";
	break;
    case DTMF_8:
	to_cout +="8";
	break;
    case DTMF_9:
	to_cout +="9";
	break;
    case DTMF_C:
	to_cout +="C";
	break;
    case DTMF_STAR:
	to_cout +="STAR";
	break;
    case DTMF_0:
	to_cout +="0";
	break;
    case DTMF_HASH:
	to_cout +="HASH";
	break;
    case DTMF_D:
	to_cout +="D";
	break;
    case DTMF_UNKNOWN:
	to_cout +="UNKNOWN";
	break;
    }
    os << to_cout;
    return os;
}
