#pragma once
#include "LocalPlayer.h"
#include "Entity.h"
#include "AimbotOptions.h"
#include <vector>

namespace TargetManager
{
	std::vector<Pawn*> GetTargetsPawn(intptr_t* pEntListBasePtr, int_least8_t* pGameTypeId);
	bool IsTargetInFov(Vector3& pTargetAngle);
	bool IsGoodTarget(Entity* pEntityPtr, int pEntIndex, int_least8_t* pGameTypeId);

	Pawn* GetNearestTarget(std::vector<Pawn*> pTargetsPawn);
	Vector3 GetTargetAngle(Vector3& pTargetsPawn);
	float GetMagnitude(const Vector3& pVec);

	void SetViewAngleSmooth(Vector3& pTargetAngle, int pSmoothValue);
	float NormalizePitch(float pPitch);
	float NormalizeYaw(float pYaw);
};