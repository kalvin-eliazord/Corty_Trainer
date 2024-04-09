#pragma once
#include "GamePointer.h"
#include "Vector3.h"
#include "Entity.h"

namespace LocalPlayer
{
	Controller* GetController();
	Pawn* GetPawn();
	float* GetPitchPtr();
	float* GetYawPtr();
	void SetViewAngle(Vector3& targetAngle);
};