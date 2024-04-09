#pragma once
#include "GamePointer.h"
#include "Controller.h"
#include "Pawn.h"

class Entity
{
private:
	Controller* controllerBase{};
	Pawn* pawnBase{};
	bool IsPawnInit{ false };
public:
	Entity(intptr_t* pContPtr);
	void SetPawnBase();
	Pawn* GetPawnBase();
	bool GetIsPawnInit();
	Controller* GetControllerBase();
};