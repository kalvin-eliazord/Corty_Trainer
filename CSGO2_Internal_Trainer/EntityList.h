#pragma once
#include "header.h"

struct Entity;

struct EntityList
{
	Entity* entity[32];
	int nbEntAlive{NULL};

	std::vector<Entity*> GetTargetList(Entity* pLocalPlayer);
	int  GetNbEntAlive();
	void SetNbEntAlive(int pNbEntAlive);
	bool IsGoodTarget(Entity* pLocalPlayer, Entity* currEntPtr);
};