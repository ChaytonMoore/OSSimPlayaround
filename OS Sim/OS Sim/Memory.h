#pragma once
#include "Page.h"
#include <iostream>


#define MAINMEMORYSIZE 512000
#define PAGESIZE 4096



class Memory
{
private:
    unsigned long _memorysize;
    char* _memory;
    unsigned long _allocatedMemory;
    unsigned long _freeMemory;



public:

    std::vector<Page*> pages;

    bool readMemory(unsigned long location, unsigned int size, char* outBuffer)
    {
        std::memcpy(outBuffer, getLocationOfMemory(location),size);


        return true;
    }


    bool writeMemory(unsigned long location, unsigned int size, char* inBuffer)
    {
        std::memcpy(getLocationOfMemory(location), inBuffer, size);


        return true;
    }

    char* getLocationOfMemory(unsigned long location)
    {
      
        return _memory + location;
    }

    Page* allocatePage(Process* requestingProcess);



    Memory();

    unsigned long getStartAddress()
    {
        return (unsigned long)&_memory;
    }


    


};

