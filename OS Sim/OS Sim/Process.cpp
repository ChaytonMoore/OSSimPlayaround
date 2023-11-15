#include "Process.h"


unsigned long Process::malloc(uint16_t size)
{


    if (PAGESIZE - ownedPages.back()->freeStart < size)
    {
        //the requested memory is larger so we need a new page

        //get it from the OS
        ownedPages.push_back(OS::getMainMemory()->allocatePage(this));

        //failed get
        if (ownedPages.back() == nullptr)
        {

            state = pState_Waiting;
            return 12; // 12 is error for insufficient memory 
        }
    }

    //always alloc
    unsigned long out = ownedPages.back()->freeStart + ownedPages.back()->startAddress;
    ownedPages.back()->freeStart += size;
    ownedPages.back()->blocks++;
    return out;
}

void Process::free(unsigned int loc, uint16_t size)
{

    size_t pIdx = 0;
    for (Page* i : ownedPages)
    {
        if (loc < PAGESIZE)
        {
            //free memory
            if (loc+size>=i->freeStart)
            {
                i->freeStart = loc;
            }
            i->blocks--;

            //if no memory left in there free the page
            if (i->blocks<1)
            {
                //free the page as well
                i->freeStart = 0; //make sure back to 0 for start of free
                i->owningProcess = nullptr;
                ownedPages.erase(ownedPages.begin(), ownedPages.begin() + pIdx); //remove
            }

            break;
        }
        else
        {
            loc -= PAGESIZE;
        }
        pIdx++;
    }

}

void Process::writeLocalMemory(unsigned int loc, unsigned int size, char* buffer)
{


    OS::getMainMemory()->writeMemory(getGlobalMemoryLocationFromLocal(loc), size, buffer);


}

void Process::readLocalMemory(unsigned int loc, char* buffer, unsigned int size)
{
    OS::getMainMemory()->readMemory(getGlobalMemoryLocationFromLocal(loc),size, buffer);


}

unsigned long Process::getGlobalMemoryLocationFromLocal(unsigned int loc)
{
    //error code is 1
    unsigned long out = 1;


    
    for(Page* i: ownedPages)
    {
        if (loc< PAGESIZE)
        {
            out = i->startAddress + loc;
            break;
        }
        else
        {
            loc -= PAGESIZE;
        }
    }


    return out;
}


Process::Process(unsigned int inID)
{
    processID = inID;
    state = pState_New;
}


