#pragma once
#include "header.h"

struct Entity;

struct EntityList
{
	Entity* entity[32];

	std::vector<Entity*> GetTargetList(Entity* pLocalPlayer, int8_t* pGameType);
	int  GetNbEntAlive();
	bool IsGoodTarget(Entity* pLocalPlayer, Entity* currEntPtr, int8_t* pGameType);
};