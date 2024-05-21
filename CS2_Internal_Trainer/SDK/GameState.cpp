#include "GameState.h"

bool GameState::IsDeathMatch()
{
	const intptr_t deathMatchPtr { GamePointers::ReadMemory(GamePointers::GetGameRulesPtr(), {MyOffset::CS_GameRules::GameModeRules, 0x30}) };
	return deathMatchPtr ? true : false;
}

bool GameState::IsMatchStarted()
{
	const int matchStateId { GamePointers::GetMatchStateId()};

	switch (matchStateId)
	{
	case IN_GAME: return true;
	case IN_LOBBY: return false;
	case NO_STATE: return false;
	default: return false;
	}
	
	return false;
}