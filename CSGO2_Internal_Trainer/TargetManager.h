#pragma once
#include "header.h"

struct Vector3;

class TargetManager
{
private:
	Vector3 oldAngleDist{
		10000.0f,
		10000.0f, 
		10000.0f };

	int oldEntityIndex{ -1 };
	float GetClamp(const float p_fValueToClamp, const float p_fMin, const float p_fMax);

public:
	void SetNearestTarget(Entity* entity, int currEntityIndex);
	int GetNearestTargetIndex();
	Vector3 GetTargetAngle(Entity* target);
};