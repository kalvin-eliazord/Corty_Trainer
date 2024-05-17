#pragma once
#include "LocalPlayer.h"
#include "Entity.h"
#include "CheatHKeys.h"
#include <vector>
#include <bitset>

namespace Aimbot
{
	inline Controller* cTargetLocked{ nullptr };

	Controller* GetCTarget();
	std::vector<Controller*> GetCTargets();

	bool IsTargetInFov(Vector3& pTargetAngle);
	bool IsGoodTarget(Entity* pEntityPtr, int pEntIndex);
	bool ImSpottedAndEntitySpotted(Entity* pCurrEnt, int pEntIndex);

	float GetMagnitude(const Vector3& pVec);
	Vector3 GetTargetAngle(Vector3 pTargetPos);
	Controller* GetNearestCTarget(std::vector<Controller*> pTargetsEnts);

	void NormalizeYaw(float& pYaw);
	void NormalizePitch(float& pPitch);
	void SetLpAngleSmooth(Vector3& pTargetAngle, const int pSmoothValue);

	bool Start();
	bool ShotLockedTarget();
	bool ShotTarget(Controller* pCTarget);
};