#pragma once
#include "header.h"

namespace LocalPlayer
{
	Entity* Get();
	void SetViewAngle(Vector3& targetAngle, const int pSmoothValue);
};