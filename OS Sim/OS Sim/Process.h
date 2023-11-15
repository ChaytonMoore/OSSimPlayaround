#pragma once
#include "OS.h"




enum processState
{
    pState_Running,
    pState_Ready,
    pState_Waiting,
    pState_New,
    pState_Terminate

};

class Process
{
public:
    unsigned int processID;
    int priority;
    processState state;
    float CPUTimeUsed;

    //using 4 for brevity 
    Page* stack[4];


    std::vector<Page*> instructionPages;

    std::vector<Page*> ownedPages;


    unsigned long malloc(uint16_t size);

    void free(unsigned int loc, uint16_t size);


    void writeLocalMemory(unsigned int loc, unsigned int size, char* buffer);

    void readLocalMemory(unsigned int loc, char* buffer, unsigned int size);

    unsigned long getGlobalMemoryLocationFromLocal(unsigned int loc);

    Process(unsigned int inID);

    

};

