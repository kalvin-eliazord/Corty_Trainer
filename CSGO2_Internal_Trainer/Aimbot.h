#pragma once
#include "header.h"

namespace Aimbot
{
	Entity* GetNearestTarget(std::vector<Entity*>);
	Vector3 GetTargetAngle(Entity* target);
};