#pragma once
#include <iostream>
#include <fstream>
#include <string>




class Compiler
{



	void compileLine(std::string in);

	void compilePrint(std::string text);

	void compileInput(int address);

	void compilePrintValue(int address);

	void compileEnable(std::string en);

	void compileEnd();


	void writeInstructionPart(uint32_t b);

	void writeInstructionPartSigned(int32_t b);

	void writeStaticInstruction(uint32_t b);

	char* functionCode;
	uint32_t functionLen;


	char* compiledData;
	int compiledLen;


public:
	void compile(bool compileToProgramFile);


};

