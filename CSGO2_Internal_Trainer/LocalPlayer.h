#pragma once
#include "Entity.h"
#include "GamePointer.h"

namespace LocalPlayer
{
	Entity* Get();
	float* GetPitchPtr();
	float* GetYawPtr();
	void SetViewAngle(Vector3& targetAngle);
};