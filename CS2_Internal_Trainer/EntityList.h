#pragma once
#include <vector>
#include "Entity.h"

struct EntityList
{
	Entity* entity[64];

	std::vector<Entity*> GetTargetList(Entity* pLocalPlayer, int8_t* pGameType);
	bool IsGoodTarget(Entity* pLocalPlayer, Entity* currEntPtr, int8_t* pGameType);
};