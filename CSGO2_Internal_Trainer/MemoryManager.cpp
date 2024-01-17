#include "MemoryManager.h"

// Get Dynamic Addr from pointer path
intptr_t MemoryManager::GetDynamicAddr(intptr_t baseAddr, std::vector<intptr_t> offsets)
{
    for (int i{ 0 }; i < offsets.size(); ++i)
        baseAddr = *(intptr_t*)(baseAddr + offsets[i]);

    return baseAddr;
}