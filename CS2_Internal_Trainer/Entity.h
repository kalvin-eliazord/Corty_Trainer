#pragma once
#include <iostream>
#include "GamePointer.h"
#include "Controller.h"
#include "Pawn.h"

struct Entity
{
	Controller* contBase{};
	Pawn* pawnBase{};
	bool isPawnInit{false};

	Entity(intptr_t* pContPtr);
	bool SetPawnBase();
};