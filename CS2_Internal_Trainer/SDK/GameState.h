#pragma once
#include <cstdint>
#include "MyPointers.h"

namespace GameState
{
	enum GameStateId
	{
		NO_STATE,
		IN_LOBBY = 4,
		IN_GAME = 8
	};

	inline bool bDefaultChange{ false };
	bool IsDeathMatch();
	bool IsMatchStarted();
};