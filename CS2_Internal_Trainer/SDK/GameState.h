#pragma once
#include <cstdint>
#include "MyOffset.h"
#include "GamePointers.h"

namespace GameState
{
	enum GameStateId
	{
		NO_STATE,
		IN_LOBBY = 4,
		IN_GAME = 8
	};

	inline bool bDefaultChange{ false };
	static constexpr intptr_t deathmatchId{ 0x7FFC52B757E0 };
	bool IsDeathMatch();
	bool IsMatchStarted();
};