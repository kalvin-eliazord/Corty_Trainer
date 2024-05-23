#include "MyPointers.h"

intptr_t MyPointers::GetEntityBase(intptr_t pIndex)
{
	const intptr_t entListBase{ static_cast<intptr_t>(ReadInternalMem<uintptr_t>(MyOffset::GPointers::ClientMod,
		{ MyOffset::GPointers::GameEntitySyst, MyOffset::EntityListPtr })) };

	constexpr intptr_t entPadding{ 0x78 };
	const intptr_t currEntIndex{ (pIndex + 1) * entPadding };
	return static_cast<intptr_t>(ReadInternalMem<uintptr_t>(entListBase, { currEntIndex }));
}

float* MyPointers::GetViewMatrixPtr()
{
	return reinterpret_cast<float*>(MyOffset::GPointers::ClientMod + MyOffset::GPointers::ViewMatrix);
}

Vector3* MyPointers::GetViewAnglesPtr()
{
	return reinterpret_cast<Vector3*>(MyOffset::GPointers::ClientMod + MyOffset::GPointers::ViewAngles);
}

intptr_t MyPointers::GetPredictionBase()
{
	return static_cast<intptr_t>(ReadInternalMem<uintptr_t>(MyOffset::GPointers::ClientMod, { MyOffset::GPointers::Prediction }));
}

intptr_t MyPointers::GetLpControllerBase()
{
	return static_cast<intptr_t>(ReadInternalMem<uintptr_t>(MyOffset::GPointers::ClientMod, { MyOffset::GPointers::LP_Controller }));
}

int* MyPointers::GetMatchStateIdPtr()
{
	return reinterpret_cast<int*>(MyPointers::GetPredictionBase() + MyOffset::IsMatchStarted);
}

intptr_t* MyPointers::GetDeathMatchRulesPtr()
{
	const intptr_t deathMatchRulesPtr{ static_cast<intptr_t>(ReadInternalMem<uintptr_t>(MyOffset::GPointers::ClientMod,
		{
			MyOffset::GPointers::GameRules,
			MyOffset::CS_GameRules::GameModeRules ,
			MyOffset::CS_GameRules::DeathMatchPtr
		}
	)) };

	return reinterpret_cast<intptr_t*>(deathMatchRulesPtr);
}

intptr_t MyPointers::GetGameEntitySystemBase()
{
	return static_cast<intptr_t>(ReadInternalMem<uintptr_t>(MyOffset::GPointers::ClientMod, { MyOffset::GPointers::GameEntitySyst }));
}