#pragma once
#include "header.h"

struct Entity;

struct EntityList
{
	std::vector<Entity*> entitiesList;

	std::vector<Entity*> GetTargetList(Entity* localPlayer, EntityList* entityList);
	bool IsGoodTarget(Entity* currEntPtr);

	int GetNbOfEntAlive();
	void PushValidEntity();
};

