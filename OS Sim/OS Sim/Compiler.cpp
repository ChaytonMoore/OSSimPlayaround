#include "Compiler.h"
#include <vector>




void Compiler::compilePrint(std::string text)
{
    //need to split string into arrays of 4 chars
    char* functionCode;

    //how to compile a print statement
    //first split the text into an array of fours and add null terminators at the end
    std::vector<char*> stringQuads;

    int fpos = 0;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (i%4==0)
        {
            char nquad[4];
            nquad[0] = text.at(i);


            stringQuads.push_back(nquad);
        }
        else
        {
            if (text.size()<=i)
            {
                fpos = i % 4;
                stringQuads.back()[i % 4] = '\0';
            }
            stringQuads.back()[i % 4] = text.at(i);
        }

    }

    if (fpos == 0)
    {
        char nquad[4];
        nquad[0] = '\0';
        nquad[1] = '\0';
        nquad[2] = '\0';
        nquad[3] = '\0';

        stringQuads.push_back(nquad);
    }

    functionCode = new char[16* stringQuads.size()+16];
    //for each quad
    for (size_t i = 0; i < stringQuads.size(); i+=16)
    {
        functionCode[i] = 19;
        functionCode[i + 4] = *stringQuads.at(i);
        functionCode[i + 8] = 1;
        functionCode[i + 12] = (uint32_t)i;


    }

    functionCode[16 * stringQuads.size()] = 23;
    functionCode[16 * stringQuads.size()+4] = *(char*)(uint32_t)4096;
    functionCode[16 * stringQuads.size() + 8] = 20;
    functionCode[16 * stringQuads.size() + 16] = *(char*)(uint16_t)8;



}

void Compiler::compileEnable(std::string en)
{
    if (en=="outputshell")
    {
        //first value of the reg might have to be a output pointer
    }
}


void Compiler::compileLine(std::string in)
{

    std::string delimiter = " ";
    std::string command = in.substr(0, in.find(delimiter));

    if (command == "print")
    {
        compilePrint(in.substr(1, in.find(delimiter)));
    }
    else if (command == "enable")
    {
        compileEnable(in.substr(1, in.find(delimiter)));
    }
}

void Compiler::compile()
{
    compiledData = new char[4096];
    compiledLen = 0;


    std::ifstream src;
    std::string line;
    src.open("shell.vosl");
    if (src.is_open())
    {
        while (std::getline(src, line))
        {
            compileLine(line);
        }
        src.close();
    }
    src.close();
}