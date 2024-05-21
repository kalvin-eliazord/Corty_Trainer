#pragma once
#include "GamePointers.h"
#include "Entity.h"
#include "Vector3.h"
#include "Aimbot.h"

namespace LocalPlayer
{
	Pawn GetPawn();
	Entity GetEntity();
	Controller GetController();
	Vector3* GetViewAnglesPtr();
	bool SetViewAngles(const Vector3& targetAngle);
	bool SetSmoothViewAngles(Vector3 pTargetAngle, const int pSmoothValue);
};