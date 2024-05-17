#pragma once
#include <cstdint>
#include "Pointer.h"
#include "Offset.h"

namespace GameState
{
	inline constexpr int16_t inGameId{ 8 };
	inline constexpr int16_t notInGameId{ 4 };
	inline int16_t oldGameStateId{ NULL };
	inline bool bDefaultChange{ false };
	bool IsGameDeathMatch();
};