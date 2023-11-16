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


	


	char* compiledData;
	int compiledLen;


public:
	void compile();


};

