#pragma once
#include "GamePointer.h"
#include "Vector3.h"
#include "Entity.h"

namespace LocalPlayer
{
	Pawn* GetPawn();
	float* GetYawPtr();
	Entity GetEntity();
	float* GetPitchPtr();
	Controller* GetController();
	void SetViewAngle(Vector3& targetAngle);
};