#include "MemoryManager.h"

// Get Dynamic Addr from pointer path
intptr_t MemoryManager::GetDynamicAddr(intptr_t* baseAddr, std::vector<intptr_t> offsets)
{
    intptr_t dynamicAddr{ NULL };

    for (int i{ 0 }; i < offsets.size(); ++i)
    {
        baseAddr += offsets[i];
        dynamicAddr = *baseAddr;
    }

    return dynamicAddr;
}