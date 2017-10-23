#include "DLL.hpp"
#include "DTMF.hpp"

#include <iostream>

int main()
{
	DLL myDLL("Hello india");
	myDLL.send("Hola");
	int var;

	return 0;
}