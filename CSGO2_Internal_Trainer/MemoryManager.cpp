#include "MemoryManager.h"

// Get Dynamic Addr from pointer path
intptr_t MemoryManager::GetDynamicAddr(const intptr_t baseAddr, std::vector<intptr_t> offsets)
{
    intptr_t dynamicAddr{ baseAddr };

    for (int i{ 0 }; i < offsets.size(); ++i)
        dynamicAddr = *(intptr_t*)(dynamicAddr + offsets[i]);

    return dynamicAddr;
}