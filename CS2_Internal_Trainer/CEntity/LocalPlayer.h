#pragma once
#include "GamePointer.h"
#include "Entity.h"
#include "Pawn.h"
#include "Controller.h"
#include "Vector3.h"

namespace LocalPlayer
{
	Pawn* GetPawn();
	float* GetYawPtr();
	Entity GetEntity();
	float* GetPitchPtr();
	Controller* GetController();
	void SetViewAngle(const Vector3& targetAngle);
};