#pragma once
#include <cstdint>

namespace GameChecker
{
	inline int* gameStateIdPtr{ nullptr };
	inline int8_t* gameTypePtr{ nullptr };
	int inline constexpr inGameId{ 8 };
	int inline constexpr notInGameId{ 4 };
};