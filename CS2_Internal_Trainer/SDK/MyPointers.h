#pragma once
#include <cstddef>
#include <vector>
#include "MyOffsets.h"
#include "Vector3.h"

namespace MyPointers
{
	float* GetViewMatrixPtr();
	int* GetMatchStateIdPtr();
	Vector3* GetViewAnglesPtr();
	intptr_t GetPredictionBase();
	intptr_t GetLpControllerBase();
	intptr_t* GetDeathMatchRulesPtr();
	intptr_t GetGameEntitySystemBase();
	intptr_t GetEntityBase(intptr_t pIndex);

	template<typename T>
	T ReadInternalMem(intptr_t pBaseAddr, std::vector<intptr_t> pListOffset)
	{
		if ((!pBaseAddr || pListOffset.empty()))
			return T();

		intptr_t addrBuffer{ pBaseAddr};
		T addrResult{};

		for (const auto& currOffset : pListOffset)
		{
			if (!currOffset) return T();

			addrBuffer = *reinterpret_cast<intptr_t*>(addrBuffer + currOffset);
			addrResult = static_cast<T>(addrBuffer);
		}

		return addrResult;
	}
}