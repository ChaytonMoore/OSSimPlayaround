#include "Compiler.h"
#include <vector>



std::vector<std::string> splitStr(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}



void Compiler::compilePrint(std::string text)
{
    //need to split string into arrays of 4 chars

    functionLen = 0;

    //how to compile a print statement
    //first split the text into an array of fours and add null terminators at the end
    std::vector<char*> stringQuads;

    int fpos = 0;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (i%4==0)
        {
            char* nquad = new char[4];
            nquad[0] = text.at(i);
            nquad[1] = '\0';
            nquad[2] = '\0';
            nquad[3] = '\0';

            stringQuads.push_back(nquad);
            fpos = 0;
        }
        else
        {

            stringQuads.back()[i % 4] = text.at(i);
            fpos = i % 4;
            

        }

    }

    if (fpos == 3)
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


    uint32_t buff1;

    for (size_t i = 0; i < stringQuads.size(); i++)
    {
        buff1 = 19;
        std::memcpy(functionCode + i*16,&buff1,4);

        std::memcpy(functionCode + i*16 + 4, stringQuads.at(i), 4);

        buff1 = 1;
        std::memcpy(functionCode + i*16 + 8, &buff1, 4);

        buff1 = i+64;
        std::memcpy(functionCode + i*16 + 12, &buff1, 4);




    }

    uint32_t oset = 16 * stringQuads.size();


    buff1 = 23;
    std::memcpy(functionCode+oset,&buff1,4);

    buff1 = 4096;
    std::memcpy(functionCode + oset + 4, &buff1, 4);

    buff1 = 20;
    std::memcpy(functionCode + oset + 8, &buff1, 4);


    uint16_t buff2 = 64;
    std::memcpy(functionCode + oset + 12, &buff2, 2);

    buff2 = stringQuads.size()*4;
    std::memcpy(functionCode + oset + 14, &buff2, 2);


    functionLen = oset + 16;


   
    //write to compiled code
    std::memcpy(compiledData+compiledLen,functionCode,functionLen);
    compiledLen += functionLen;
    delete[] functionCode;

}

void Compiler::compileInput(int address)
{
    functionCode = new char[64];
     functionLen = 0;

    uint32_t buff1 = 21;
    std::memcpy(functionCode + functionLen, &buff1, 4);
    functionLen += 4;

    uint16_t buff2 = address;
    std::memcpy(functionCode + functionLen, &buff2, 2);
    buff2 = 256;
    std::memcpy(functionCode + functionLen + 2, &buff2, 2);
    functionLen += 4;


    std::memcpy(compiledData + compiledLen, functionCode, functionLen);
    compiledLen += functionLen;
    delete[] functionCode;


}

void Compiler::compilePrintValue(int address)
{
    functionCode = new char[256];
    functionLen = 0;


    //load into the acc the constant value
    uint32_t buff = 19;
    memcpy(functionCode + functionLen, &buff,4);
    functionLen += 4;

    buff = address;
    memcpy(functionCode + functionLen, &buff, 4);
    functionLen += 4;


    //load that into a memory address 128, this is the start value
    buff = 1;
    memcpy(functionCode + functionLen, &buff, 4);
    functionLen += 4;

    buff = 128;
    memcpy(functionCode + functionLen, &buff, 4);
    functionLen += 4;


    //copy this value into 129 as this will be the sort of length value.
    buff = 1;
    memcpy(functionCode + functionLen, &buff, 4);
    functionLen += 4;

    buff = 129;
    memcpy(functionCode + functionLen, &buff, 4);
    functionLen += 4;

   


    buff = 19;
    memcpy(functionCode+functionLen, &buff,4);
    functionLen += 4;

    buff = 129;
    memcpy(functionCode + functionLen, &buff, 4);
    functionLen += 4;

    

    buff = 27;
    memcpy(functionCode + functionLen, &buff, 4);
    functionLen += 8;

    buff = 25;
    memcpy(functionCode + functionLen, &buff, 4);
    functionLen += 4;

    int buff2 = 3;
    memcpy(functionCode + functionLen, &buff2, 4);
    functionLen += 4;

    writeInstructionPart(26); //increment registry
    writeInstructionPart(129);

    writeInstructionPart(24);
    writeInstructionPartSigned(-4);



    //end found



    writeInstructionPart(1); //read 129 into acc
    writeInstructionPart(129);

    writeInstructionPart(3); // subtract r128 from r129
    writeInstructionPart(128);


    writeInstructionPart(1); //write acc to r130, this is the length
    writeInstructionPart(130);


    //now need to write from cache to global ram output buffer



}



void Compiler::compileEnd()
{
    uint32_t buff = 22;
    memcpy(compiledData+compiledLen, &buff,4);
    compiledLen += 4;

}

void Compiler::writeInstructionPart(uint32_t b)
{
    memcpy(functionCode + functionLen, &b, 4);
    functionLen += 4;
}

void Compiler::writeInstructionPartSigned(int32_t b)
{
    memcpy(functionCode + functionLen, &b, 4);
    functionLen += 4;
}

void Compiler::writeStaticInstruction(uint32_t b)
{
    memcpy(functionCode + functionLen, &b, 4);
    functionLen += 8;
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
        compilePrint(splitStr(in," ").at(1));
    }
    else if (command == "enable")
    {
        compileEnable(splitStr(in, " ").at(1));
    }
    else if (command == "input")
    {
        compileInput(std::stoi(splitStr(in," ").at(1)));
    }
    else if (command == "end")
    {
        compileEnd();
    }
}

void Compiler::compile(bool compileToProgramFile)
{


    std::cout << "Starting Compiling" << std::endl;


    compiledData = new char[4096];
    compiledLen = 0;

    std::ifstream src;
    std::string line;
    src.open("test.vosl");
    if (src.is_open())
    {
        while (std::getline(src, line))
        {
            compileLine(line);
        }
        src.close();

    }
    src.close();

    if (compileToProgramFile)
    {


        //write code to file

        std::ofstream outputFile;
        outputFile.open("test.vosp", std::ios::binary | std::ios::out);

        int32_t priority = 101;
        uint32_t codechunks = 1;

        outputFile.write((char*)&priority, 4);
        outputFile.write((char*)&codechunks, 4);


        outputFile.write((char*)&compiledLen, 4);
        outputFile.write(compiledData, compiledLen);
        outputFile.close();
    }
    else
    {


        //write code to file

        std::ofstream outputFile;
        outputFile.open("test.vosmc", std::ios::binary | std::ios::out);

        outputFile.write((char*)&compiledLen, 4);
        outputFile.write(compiledData, compiledLen);
        outputFile.close();
    }

    std::cout << "Finished Compiling"<<std::endl;



}
