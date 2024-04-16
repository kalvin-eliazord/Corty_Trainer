#pragma once
#include "LocalPlayer.h"
#include "Entity.h"
#include "CheatOptions.h"
#include <vector>
#include <bitset>

namespace TargetManager
{
	Controller* GetCTarget();
	std::vector<Controller*> GetCTargetsEnts();

	bool IsTargetInFov(Vector3& pTargetAngle);
	bool IsGameDeathMatch(int16_t* pGameTypeId);
	bool ImSpottedAndEntitySpotted(Entity* pCurrEnt, int pEntIndex);
	bool IsGoodTarget(Entity* pEntity, int pEntIndex);

	float GetMagnitude(const Vector3& pVec);
	Vector3 GetTargetAngle(Vector3 pTargetPos);
	Controller* GetNearestCTarget(std::vector<Controller*> pTargetsEnts);

	float NormalizePitch(float pPitch);
	float NormalizeYaw(float pYaw);
	void SetViewAngleSmooth(Vector3& pTargetAngle, int pSmoothValue);
};