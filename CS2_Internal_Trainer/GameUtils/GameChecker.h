#pragma once
#include <cstdint>

namespace GameChecker
{
	inline bool bGamePointerInit{false};
	inline constexpr int16_t inGameId{ 8 };
	inline constexpr int16_t notInGameId{ 4 };
	inline int16_t oldGameStateId{ NULL };
};