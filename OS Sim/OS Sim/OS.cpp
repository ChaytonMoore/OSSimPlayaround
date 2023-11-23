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
	std::ifstream ROM("C:\\Users\\21441572\\Source\\Repos\\ChaytonMoore\\OSSimPlayaround\\OS Sim\\OS Sim\\ROM", std::ios::in | std::ios::binary);
	
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
	nProc->state = processState::pState_New;

	return nProc;
}

Process* OS::StartProcessFromFile(std::string fileName)
{

	//read in file
	
	std::ifstream pFile(fileName, std::ios::in | std::ios::binary);

	//read in meta data

	__int32 priority;
	pFile.read((char*)&priority, 4);

	unsigned __int32 codeBlocks;
	pFile.read((char*)&codeBlocks, 4);


	
	unsigned int hashedID = rand();
	
	//start process

	Process* nProc = StartProcess(hashedID,priority);

	uint32_t codeBlockLen;
	char* codeBuffer;


	for (size_t i = 0; i < codeBlocks; i++)
	{
		pFile.read((char*)&codeBlockLen,4);

		codeBuffer = new char[codeBlockLen];
		pFile.read(codeBuffer,codeBlockLen);

		//if more than 1 code block then add new instruction page
		if (i>0)
		{
			nProc->instructionPages.push_back(_MainMemory->allocatePage(nProc));
		}


		std::memcpy(nProc->instructionPages.at(i), codeBuffer, codeBlockLen);

		delete[] codeBuffer;
	}




	return nProc;
}

void OS::takeInput(char* in, size_t len)
{
	memcpy(_MainMemory->getLocationOfMemory(_IOHandling->stack[0]->startAddress),in,len);


}

void OS::processStateRecalc()
{
	//move new processes to ready stack


	for (size_t i = 0; i < processes.size(); i++)
	{
		if (processes[i]->state==pState_New)
		{
			processes[i]->state = pState_Ready;

			//add to cpu queue
			_Processor->readyProcesses.push(processes[i]);
		}

		if (processes[i]->state==pState_Terminate)
		{
			processes.erase(processes.begin()+i);
			i--; // decrement to re align i
		}


	}


}

void OS::processorPass()
{
	processStateRecalc();


	if (_Processor->currentProcess->state==pState_Waiting)
	{
		_Processor->resumeProgram();
	}
	else if (_Processor->readyProcesses.size())
	{
		Process* pToRun = _Processor->readyProcesses.front();

		if (pToRun != nullptr)
		{
			_Processor->readyProcesses.pop(); //remove from queue

			_Processor->runProgram(pToRun);
			
		}
	}





}
