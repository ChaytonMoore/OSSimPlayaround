#include <queue>



class Process;
class OS;

#pragma once
class CPU
{
	friend class OS;
public:


	Process* currentProcess;

	void runProgram(Process* newProgram);

	void resumeProgram();

	std::queue<Process*> readyProcesses;
	std::queue<Process*> waitingProcesses;
	



private:
	unsigned int _counter;
	int _accumulator;
	unsigned int _writeaddressregister;
	unsigned int _readaddressregister;
	unsigned long _ramAddressRegister;
	int _registers[1024];
	__int32 _instructions[2048];

	int runInstruction();



};

