#include "Memory.h"




Memory::Memory()
{


    _memorysize = MAINMEMORYSIZE;
    _memory = new char[_memorysize];
    _allocatedMemory = 0;
    _freeMemory = _memorysize;


    //set up pages

    for (size_t i = 0; i < _memorysize / PAGESIZE; i++)
    {
        Page* newPage = new Page(i * PAGESIZE);
        pages.push_back(newPage);
    }


    //set up OS owned first page 

}

Page* Memory::allocatePage(Process* requestingProcess)
{
    for (Page* i : pages)
    {
        if (i->owningProcess == nullptr)
        {
            i->owningProcess = requestingProcess;
            return i;
        }
    }

    //if can't get unassigned page then return a fail
    return nullptr;
}