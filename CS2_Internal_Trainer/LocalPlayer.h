#pragma once
#include "Vector3.h"
#include "GamePointer.h"

namespace LocalPlayer
{
	intptr_t* Get();
	float* GetPitchPtr();
	float* GetYawPtr();
	void SetViewAngle(Vector3& targetAngle);
};