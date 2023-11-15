#pragma once
#include "Memory.h"
#include "CPU.h"

class Process;

class OS
{
private:
	static Memory* _MainMemory;
	static CPU* _Processor;
	static Process* _IOHandling;
	



public:



	static Memory* getMainMemory()
	{
		return _MainMemory;
	}

	static Process* getInput()
	{
		return _IOHandling;
	}

	static void bootOS();

	static char* readOutputBuffer();

	static std::vector<Process*> processes;
	static Process* BootProcess;


	static Process* StartProcess(unsigned int ID, int priority);


	static void takeInput(char* in, size_t len);
};

