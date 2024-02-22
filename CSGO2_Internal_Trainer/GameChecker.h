#pragma once
#include "header.h"

struct EntityList;

namespace GameChecker
{
	static int* gameStateIdPtr{ (int*)(GameOffset::Client::gameStateIdPtr) };
	constexpr static int inGameId{ 8 };
	void CheckGameState(EntityList* pEntityList);
};