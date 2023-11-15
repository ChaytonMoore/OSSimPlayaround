#pragma once
#include<vector>


class Process;



struct Page
{
public:
    unsigned long startAddress;
    Process* owningProcess;
    uint16_t freeStart;
    uint16_t blocks;

    Page(unsigned long inAddress)
    {
        startAddress = inAddress;
        freeStart = 0;
        blocks = 0;
        owningProcess = nullptr;
    }

};