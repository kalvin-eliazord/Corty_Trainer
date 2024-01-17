#pragma once
#include "header.h"

namespace Offset
{
	static const intptr_t clientModBaseAddr{ (intptr_t) GetModuleHandleW(L"client.dll") };
	static const intptr_t engineModBaseAddr{ (intptr_t) GetModuleHandleW(L"engine2.dll") };
	static constexpr intptr_t localPlayer{ 0x16C8F58 };
	static constexpr intptr_t entitiesList{ 0x16D5C78 };
	static constexpr intptr_t cPredictionBaseAddr{ 0x542C48 };
	static constexpr intptr_t gameState{ 0xA0 };
};