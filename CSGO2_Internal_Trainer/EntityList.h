#pragma once
#include "header.h"

struct Entity;

struct EntityList
{
	Entity* entity[32];
	int nbEntities{};

	std::vector<Entity*> GetTargetList();
	int  GetNbEntAlive();
	void SetNbEntAlive();
	bool IsGoodTarget(Entity* currEntPtr);
};