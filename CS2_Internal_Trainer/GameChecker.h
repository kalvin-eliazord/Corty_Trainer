#pragma once
#include <cstdint>

namespace GameChecker
{
	static bool bGamePointerInit{false};
	static constexpr int_least8_t inGameId{ 8 };
	static constexpr int_least8_t notInGameId{ 4 };
	inline int_least8_t oldGameStateId{ NULL };
};