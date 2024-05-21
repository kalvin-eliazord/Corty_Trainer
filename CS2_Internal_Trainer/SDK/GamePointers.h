#pragma once
#include "MyOffset.h"
#include <Windows.h>
#include "Vector3.h"
#include <vector>
#include <map>
#include <string>

namespace GamePointers
{
	inline std::map<std::string_view, intptr_t> pointersState;
	const intptr_t GetClientMod();
	const intptr_t GetInputMod();

	Vector3* GetViewAnglesPtr();
	float* GetViewMatrixPtr();
	intptr_t GetEntListPtr();
	intptr_t GetGameRulesPtr();
	intptr_t GetLpControllerPtr();
	intptr_t GetGameEntitySystemPtr();
	intptr_t* GetEntityPtr(intptr_t pIndex);
	int GetMatchStateId();

	bool ArePointersInit();
	intptr_t ReadMemory(intptr_t pBaseAddr, std::vector<intptr_t> pListOffset);
};