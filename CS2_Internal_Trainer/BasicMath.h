#pragma once
#include <math.h>
#include "Vector3.h"

namespace BasicMath
{
	float GetMagnitude(const Vector3& pValue);
	float GetMagnitude(const Vector3& pV1, const Vector3& pV2);
	Vector3 GetDelta(const Vector3& pV1, const Vector3& pV2);
};