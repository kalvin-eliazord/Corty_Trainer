#pragma once
#include <cstdint>

namespace GameChecker
{
	static bool bGamePointerInit{false};
	static constexpr int16_t inGameId{ 8 };
	static constexpr int16_t notInGameId{ 4 };
	inline int16_t oldGameStateId{ NULL };
};