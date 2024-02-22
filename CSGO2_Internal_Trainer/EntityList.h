#pragma once
#include "header.h"

struct Entity;

struct EntityList
{
	Entity* entity[32];
	int nbEntAlive{};

	std::vector<Entity*> GetTargetList();
	int  GetNbEntAlive();
	void  SetNbEntAlive(int pNbEntAlive);
	void  SetNbEntAlive();
	bool IsGoodTarget(Entity* currEntPtr);
};