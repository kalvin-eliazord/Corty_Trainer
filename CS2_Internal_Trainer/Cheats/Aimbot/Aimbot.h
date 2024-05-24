#pragma once
#include "MyPointers.h"
#include "LocalPlayer.h"
#include "Entity.h"
#include "CheatHKeys.h"
#include <vector>
#include <bitset>

namespace Aimbot
{
	inline Entity cTargetLocked{};

	bool GetEntTarget(Entity& pEntityTarget);
	std::vector<Entity> GetValidTargets();

	bool IsTargetInFov(Vector3& pTargetAngle);
	bool IsGoodTarget(Entity* pEntityPtr, int pEntIndex);
	bool IsSpotted(Entity* pCurrEnt, int pEntIndex);

	float GetMagnitude(const Vector3& pVec);
	Vector3 GetTargetAngle(Vector3 pTargetPos);
	Entity GetNearestTarget(std::vector<Entity> pTargetsEnts);

	void NormalizeYaw(float& pYaw);
	void NormalizePitch(float& pPitch);

	bool Start();
	bool ShotLockedTarget();
	bool ShotTarget(const Entity& pCTarget);
};