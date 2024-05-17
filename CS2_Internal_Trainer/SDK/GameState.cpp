#include "GameState.h"

bool GameState::IsGameDeathMatch()
{
	constexpr intptr_t deathmatchId{ 0x7FFC52B757E0 };

	const intptr_t gameModeRulesId{ *reinterpret_cast<intptr_t*>(reinterpret_cast<intptr_t>(Pointer::gameRules) + Offset::GameModeRules) };

	return gameModeRulesId == deathmatchId ? true : false;
}