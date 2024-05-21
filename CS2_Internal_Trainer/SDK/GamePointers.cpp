#include "GamePointers.h"

const intptr_t GamePointers::GetClientMod()
{
	const intptr_t clientMod{ reinterpret_cast<intptr_t>(GetModuleHandleW(L"client.dll")) };
	return clientMod;
}

const intptr_t GamePointers::GetInputMod()
{
	const intptr_t inputMod{ reinterpret_cast<intptr_t>(GetModuleHandleW(L"inputsystem.dll")) };
	return inputMod;
}

intptr_t GamePointers::ReadMemory(const intptr_t pBaseAddr, std::vector<intptr_t> pListOffset)
{
	if ((!pBaseAddr || pListOffset.empty()))
		return NULL;

	intptr_t pAddrResult{ pBaseAddr };

	for (const auto& currOffset : pListOffset)
	{
		if (!currOffset) return NULL;

		pAddrResult += currOffset;
		pAddrResult = *reinterpret_cast<intptr_t*>(pAddrResult);
	}

	return pAddrResult;
};

intptr_t GamePointers::GetGameEntitySystemPtr()
{
	return ReadMemory(GetClientMod(), { cs2_dumper::offsets::client_dll::dwGameEntitySystem });
}
intptr_t GamePointers::GetGameRulesPtr()
{
	return ReadMemory(GetClientMod(), { cs2_dumper::offsets::client_dll::dwGameRules });
}

intptr_t GamePointers::GetLpControllerPtr()
{
	return ReadMemory(GetClientMod(), { cs2_dumper::offsets::client_dll::dwLocalPlayerController });
}

float* GamePointers::GetViewMatrixPtr()
{
	return reinterpret_cast<float*>(GetClientMod() + cs2_dumper::offsets::client_dll::dwViewMatrix);
}

Vector3* GamePointers::GetViewAnglesPtr()
{
	return reinterpret_cast<Vector3*>(GetClientMod() + cs2_dumper::offsets::client_dll::dwViewAngles);
}

intptr_t GamePointers::GetEntListPtr()
{
	return ReadMemory(GetGameEntitySystemPtr(), {MyOffset::EntityListPtr });
}

intptr_t* GamePointers::GetEntityPtr(intptr_t pIndex)
{
	return reinterpret_cast<intptr_t*>(GetEntListPtr() + (pIndex + 1) * 0x78);
}

int GamePointers::GetMatchStateId()
{
	return *reinterpret_cast<int*>(GetClientMod() + cs2_dumper::offsets::client_dll::dwPrediction + MyOffset::IsMatchStarted);
}

bool GamePointers::ArePointersInit()
{
	pointersState["GameEntitySystem"] = GetGameEntitySystemPtr();

	pointersState["CSGameRules"] = GetGameRulesPtr();

	pointersState["LpController"] = GetLpControllerPtr();

	pointersState["ViewMatrix"] = reinterpret_cast<intptr_t>(GetViewMatrixPtr());

	pointersState["ViewAnglesPtr"] = reinterpret_cast<intptr_t>(GetViewAnglesPtr());

	for (const auto& pointer : pointersState)
	{
		if (!pointer.second) return false;
	}

	return true;
}