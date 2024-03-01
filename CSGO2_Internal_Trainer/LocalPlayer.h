#pragma once
#include "header.h"

namespace LocalPlayer
{
	Entity* Get();
	void SetViewAngle(Vector3& lpAngle, Vector3& targetAngle, const int pSmoothValue);
};