#pragma once
#include "header.h"
namespace MemoryManager
{
	intptr_t GetDynamicAddr(intptr_t baseAddr, std::vector<intptr_t> offsets);
};
