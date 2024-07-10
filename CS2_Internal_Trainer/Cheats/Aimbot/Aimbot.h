#pragma once
#include "MyPointers.h"
#include "LocalPlayer.h"
#include "Entity.h"
#include "ConsoleMenu.h"
#include <vector>
#include <bitset>

namespace Aimbot
{
	inline Entity cTargetLocked{};

	Entity GetEntTarget(const std::vector<Entity>& pTargets);
	std::vector<Entity> GetValidTargets();

	bool IsTargetInFov(Vector3& pTargetAngle);
	bool IsSpotted(Entity pCurrEnt);

	float GetMagnitude(const Vector3& pVec);
	Vector3 GetTargetAngle(Vector3 pTargetPos);
	Entity GetNearestTarget(std::vector<Entity> pTargetsEnts);

	void NormalizeYaw(float& pYaw);
	void NormalizePitch(float& pPitch);

	bool Start(const std::vector<Entity>& pTargets);
	bool ShotLockedTarget();
	bool ShotTarget(const Entity& pCTarget);
};