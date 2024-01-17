#pragma once
#include "header.h"

struct Vector3;

class Aimbot
{
private:
	float oldNearestTarget{ 10000.0f};
	int oldEntityIndex{ -1 };

public:
	int GetNearestTarget(Entity* entity, int currEntityIndex);
	Vector3 GetTargetAngle(Entity* target);
};