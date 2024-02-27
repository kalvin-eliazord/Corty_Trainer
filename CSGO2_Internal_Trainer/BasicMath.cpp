#include "BasicMath.h"

float BasicMath::GetMagnitude(const Vector3& pValue)
{
	return { (pValue.x + pValue.y + pValue.z) };
}

float BasicMath::GetMagnitude(const Vector3& pV1, const Vector3& pV2)
{
	const Vector3 delta{ BasicMath::GetDelta(pV1, pV2) };

	return {(delta.x + delta.y + delta.z)};
}

Vector3 BasicMath::GetDelta(const Vector3& pV1, const Vector3& pV2)
{
	return {(pV1.x - pV2.x),
			(pV1.y - pV2.y),
			(pV1.z - pV2.z)};
}
