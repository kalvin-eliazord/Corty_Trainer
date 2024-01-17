#pragma once
#include "header.h"

struct Entity;

struct EntityList
{
	Entity* entity[32];

	int GetNbEntAlive();
	bool IsGoodTarget(Entity* currEntPtr);
};