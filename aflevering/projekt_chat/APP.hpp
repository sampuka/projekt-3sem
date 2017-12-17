#pragma once
#include "DLL.hpp"
#include "DTMF.hpp"

#include <iostream>
#include <random>
#include <thread>
#include <mutex>

using namespace std;

class APP
{
public:
	APP();
	void start();
	~APP();
};

