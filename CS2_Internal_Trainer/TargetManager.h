#pragma once
#include <vector>
#include "LocalPlayer.h"
#include "Entity.h"

namespace TargetManager
{
	float GetMagnitude(Vector3 pVec);
	Entity* GetNearestTarget(Entity* pLocalPlayer, std::vector<Entity*> pTargetList);
	Vector3 GetTargetAngle(Entity* pLocalPlayer, Entity* target);
	void SetViewAngleSmooth(Vector3& pTargetAngle, const int pSmoothValue);
	float NormalizePitch(const float pPitch);
	float NormalizeYaw(float pYaw);
};