#pragma once
#include <vector>
#include "LocalPlayer.h"
#include "Entity.h"
#include "BasicMath.h"

namespace TargetManager
{
	float GetClamp(const float p_fValueToClamp, const float p_fMin, const float p_fMax);
	Entity* GetNearestTarget(Entity* pLocalPlayer, std::vector<Entity*> pTargetList);
	Vector3 GetTargetAngle(Entity* pLocalPlayer, Entity* target);
	void SetViewAngleSmooth(Vector3& pTargetAngle, const int pSmoothValue);
};