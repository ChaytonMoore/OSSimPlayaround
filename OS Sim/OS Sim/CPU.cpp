#include "CPU.h"
#include <cstring>
#include "Process.h"

void CPU::runProgram(Process* newProgram)
{


	//set current program
	currentProcess = newProgram;
	currentProcess->state = pState_Running;

	//then load the first page of instructions which is half of the cache

	std::memcpy(&_instructions,currentProcess->instructionPages.at(0),2048);
	

	//clear registers
	_counter = 0;
	_accumulator = 0;

	//start running 
	int ts = 0;
	while (true)
	{
		ts = runInstruction();
		if (ts!=0)
		{
			break;
		}
	}

	if (ts==1)
	{
		currentProcess->state = pState_Terminate;
	}
	else if (ts==2)
	{
		currentProcess->state = pState_Waiting;
	}



}

void CPU::resumeProgram()
{
	int ts = 0;
	while (true)
	{
		ts = runInstruction();
		if (ts!=0)
		{
			break;
		}
	}

	if (ts == 1)
	{
		currentProcess->state = pState_Terminate;
	}
	else if (ts == 2)
	{
		currentProcess->state = pState_Waiting;
	}
	


}



int CPU::runInstruction()
{

	//runs instructions


	//0: loads the data at the data register index of the next byte of instruction
	//1: stores the current acc value at the index opposite of load
	//2: adds value at the instrucion index to the acc
	//3: subtracts 
	//4: bitwise and
	//5: bitwise xor
	//6: jump if acc 0 or less
	//7: jump 
	//8: write memory to ram
	//9: read memory from ram
	//10: bitwise or
	//11: add constant(instruction value instead of index)
	//12: jump if acc 0 
	//13: read instruction from ram
	//14: malloc ram at size of write address register and sets address of ram register
	//15: free ram at loc write address register of size accumulator
	//16: load value into WAR
	//17: load into RAR
	//18: load into RAMAR
	//19: set the acc to the instruction data value
	//20: write to global ram location, only useful for system stuff
	//21: read global ram location
	//22: halt
	//23: ram address from instruction
	//24: jump relative
	//25: jump if acc 0 relative
	//26: increment registry
	//27: set acc to register value at acc idx
	

	//how 8 and 9 works
	/*
	8
		split instruction data in half. first half is register loc and send is size
		then put this in local process memory at address in write register

	9
		similar to 8
	
	
	*/

	//predefining variables needed in the switch
	unsigned __int16 tLoc;
	unsigned __int16 tSize;
	char* tBuffer;

	switch (_instructions[_counter*2])
	{
		case 0:
			_accumulator = _registers[_instructions[_counter * 2 + 1]];
			_counter++;
		break;

		case 1:
			_registers[_instructions[_counter * 2 + 1]] = _accumulator;
			_counter++;
			break;

		case 2:
			_accumulator += _registers[_instructions[_counter * 2 + 1]];
			_counter++;
			break;

		case 3:
			_accumulator -= _registers[_instructions[_counter * 2 + 1]];
			_counter++;
			break;


		case 4:
			_accumulator = _accumulator & _registers[_instructions[_counter * 2 + 1]];
			_counter++;
			break;

		case 5:
			_accumulator = _accumulator ^ _registers[_instructions[_counter * 2 + 1]];
			_counter++;

			break;

		case 6:

			if (_accumulator < 1)
			{
				_counter = _registers[_instructions[_counter * 2 + 1]];
			}
			

			break;

		case 7:

			_counter = _registers[_instructions[_counter * 2 + 1]];

			break;



		case 8:



			tLoc = ((unsigned __int16*)(&_instructions[_counter * 2 + 1]))[0];
			tSize = ((unsigned __int16*)(&_instructions[_counter * 2 + 1]))[1];

			//copy memory
			tBuffer = new char[tSize];
			std::memcpy(tBuffer,&_registers[tLoc], tSize);

			
			//interact with the process to write memory
			currentProcess->writeLocalMemory(_writeaddressregister, tSize, tBuffer);
			delete[] tBuffer; // deleted after use

			_counter++;
			break;

		case 9:



			tLoc = ((unsigned __int16*)(&_instructions[_counter * 2 + 1]))[0];
			tSize = ((unsigned __int16*)(&_instructions[_counter * 2 + 1]))[1];

			tBuffer = new char[tSize];

			currentProcess->readLocalMemory(_readaddressregister, tBuffer, tSize);

			std::memcpy(&_registers[tLoc], tBuffer, tSize);
			delete[] tBuffer;

			_counter++;
			break;
		case 10:
			_accumulator = _accumulator | _registers[_instructions[_counter * 2 + 1]];
			_counter++;

			break;

		case 11:
			_accumulator += _instructions[_counter * 2 + 1];
			_counter++;
			break;

		case 12:

			if (_accumulator == 0)
			{
				_counter = _registers[_instructions[_counter * 2 + 1]];
			}


			break;

		case 13:
			tLoc = ((unsigned __int16*)(&_instructions[_counter * 2 + 1]))[0];
			tSize = ((unsigned __int16*)(&_instructions[_counter * 2 + 1]))[1];

			tBuffer = new char[tSize];

			currentProcess->readLocalMemory(_readaddressregister, tBuffer, tSize);

			std::memcpy(&_instructions[tLoc], tBuffer, tSize);
			delete[] tBuffer;

			_counter++;
			break;

		case 14:

			_ramAddressRegister =  currentProcess->malloc(_writeaddressregister);
			_counter++;
			break;

		case 15:
			currentProcess->free(_writeaddressregister,_accumulator);
			_counter++;
			break;
		case 16:
			_writeaddressregister = _registers[_instructions[_counter * 2 + 1]];
			_counter++;
			break;
		case 17:
			_readaddressregister = _registers[_instructions[_counter * 2 + 1]];
			_counter++;
			break;
		case 18:
			_ramAddressRegister = _registers[_instructions[_counter * 2 + 1]];
			_counter++;
			break;

		case 19:
			_accumulator = _instructions[_counter * 2 + 1];
			_counter++;
			break;

		case 20:



			tLoc = ((unsigned __int16*)(&_instructions[_counter * 2 + 1]))[0];
			tSize = ((unsigned __int16*)(&_instructions[_counter * 2 + 1]))[1];

			//copy memory
			tBuffer = new char[tSize];
			std::memcpy(tBuffer, &_registers[tLoc], tSize);


			//interact with the process to write memory
			OS::getMainMemory()->writeMemory(_ramAddressRegister, tSize, tBuffer);
			delete[] tBuffer; // deleted after use

			_counter++;
			break;

		case 21:



			tLoc = ((unsigned __int16*)(&_instructions[_counter * 2 + 1]))[0];
			tSize = ((unsigned __int16*)(&_instructions[_counter * 2 + 1]))[1];

			tBuffer = new char[tSize];

			OS::getMainMemory()->readMemory(tLoc,tSize,tBuffer);

			std::memcpy(&_registers[tLoc], tBuffer, tSize);
			delete[] tBuffer;

			_counter++;
			return 2; //return for a memory read
		case 22:
			//halt
			return 1;

		case 23:
			_ramAddressRegister = _instructions[_counter * 2 + 1];
			_counter++;
			break;



		case 24:

			_counter += _instructions[_counter * 2 + 1];

			break;


		case 25:

			if (_accumulator == 0)
			{
				_counter += _instructions[_counter * 2 + 1];
			}


			break;

		case 26:

			_registers[_instructions[_counter * 2 + 1]]++;
			_counter++;


		case 27:
			_accumulator = _registers[_accumulator];




		default:
			return 1;

	}




	return 0;

}
