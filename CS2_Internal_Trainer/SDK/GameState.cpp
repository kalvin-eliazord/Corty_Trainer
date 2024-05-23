#include "GameState.h"

bool GameState::IsDeathMatch()
{
	const intptr_t* deathMatchPtr{ MyPointers::GetDeathMatchRulesPtr() };
	return deathMatchPtr ? true : false;
}

bool GameState::IsMatchStarted()
{
	const int matchStateId { *MyPointers::GetMatchStateIdPtr()};

	switch (matchStateId)
	{
	case IN_GAME: return true;
	case IN_LOBBY: return false;
	case NO_STATE: return false;
	default: return false;
	}
	
	return false;
}