#pragma once
#include <iostream>
#include <fstream>
#include <string>




class Compiler
{
	void compileLine(std::string in);

	void compilePrint(std::string text);

	void compileEnable(std::string en);


	char* compiledData;
	int compiledLen;


public:
	void compile();


};

