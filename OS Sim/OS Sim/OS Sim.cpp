#include "Process.h"
#include "Compiler.h"
#include <iostream> 




bool isCompilerMode = false;

int main()
{

	if (isCompilerMode)
	{
		Compiler* comp = new Compiler();
		comp->compile(true);
		return 0;
	}
	OS::bootOS();

	char inBuff[128];



	OS::StartProcessFromFile("test.vosp");

	while (true)
	{
		



		system("cls");
		std::cout << OS::readOutputBuffer();
		
		std::cin.getline(inBuff, 128);
		OS::takeInput(inBuff,128);
		


		//run os loop
		OS::processorPass();
		
		
	}




}


