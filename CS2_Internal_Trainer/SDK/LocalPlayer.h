#pragma once
#include "Pointer.h"
#include "Entity.h"
#include "Vector3.h"

namespace LocalPlayer
{
	Pawn* GetPawn();
	Entity GetEntity();
	float* GetYawPtr();
	float* GetPitchPtr();
	Controller* GetController();
	void SetViewAngle(const Vector3& targetAngle);
};