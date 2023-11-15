#include "Process.h"
#include "Compiler.h"
#include <iostream> 




bool isCompilerMode = true;

int main()
{

	if (isCompilerMode)
	{
		Compiler* comp = new Compiler();
		comp->compile();
		return 0;
	}
	OS::bootOS();

	char inBuff[128];

	while (true)
	{
		
		system("cls");
		std::cout << OS::readOutputBuffer();
		
		std::cin.getline(inBuff, 128);
		OS::takeInput(inBuff,128);
		
		
		
	}




}


