#include "OS.h"
#include "Process.h"
#include <fstream>
#include <iostream>

Memory* OS::_MainMemory = new Memory();

CPU* OS::_Processor = new CPU();

Process* OS::_IOHandling = new Process(5);

std::vector<Process*> OS::processes;

Process* OS::BootProcess;




void OS::bootOS()
{
	//initiate input and output memory

	//first stack page is for input second is for output

	_IOHandling->stack[0] = _MainMemory->allocatePage(_IOHandling);
	_IOHandling->stack[1] = _MainMemory->allocatePage(_IOHandling);


	//The first and second memory blocks are for IO


	//boot requires reading from ROM
	char fBuffer[4];
	std::ifstream ROM("D:\\Source\\OS Sim\\OS Sim\\ROM", std::ios::in | std::ios::binary);
	
	ROM.read(fBuffer, 4);
	int restOfFileLength = *(int*)fBuffer;

	char* mBuffer = new char[restOfFileLength];
	ROM.read(mBuffer,restOfFileLength);

	//need a new process for the booting
	BootProcess = StartProcess(1, 10);
	std::memcpy(BootProcess->instructionPages.at(0),mBuffer,restOfFileLength);
	_Processor->runProgram(BootProcess);
	
	

}

char* OS::readOutputBuffer()
{
	char* out = _MainMemory->getLocationOfMemory(_IOHandling->stack[1]->startAddress);
	
	return out;
	
}

Process* OS::StartProcess(unsigned int ID, int priority)
{
	Process* nProc = new Process(ID);
	nProc->priority = priority;
	processes.push_back(nProc);

	//init stack
	nProc->stack[0] = _MainMemory->allocatePage(nProc);
	nProc->stack[1] = _MainMemory->allocatePage(nProc);
	nProc->stack[2] = _MainMemory->allocatePage(nProc);
	nProc->stack[3] = _MainMemory->allocatePage(nProc);

	//allocate a single instruction page
	nProc->instructionPages.push_back(_MainMemory->allocatePage(nProc));

	return nProc;
}

void OS::takeInput(char* in, size_t len)
{
	memcpy(_MainMemory->getLocationOfMemory(_IOHandling->stack[0]->startAddress),in,len);


}
