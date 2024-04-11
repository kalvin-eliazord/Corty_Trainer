#include "TargetManager.h"

bool TargetManager::IsTargetInFov(Vector3& pTargetAngle)
{
	Vector3 localPlayerAngle{ LocalPlayer::GetPawn()->angles };

	Vector3 deltaAngle{};

	if(localPlayerAngle.y > pTargetAngle.y)
		deltaAngle = pTargetAngle - localPlayerAngle;
	else
		deltaAngle = localPlayerAngle - pTargetAngle;

	// Checking if target is in FOV
	if ((deltaAngle.x) < AimbotOptions::fovValue or
		-deltaAngle.x > -AimbotOptions::fovValue)
	{
		if ((deltaAngle.y) < AimbotOptions::fovValue or
			-deltaAngle.y > -AimbotOptions::fovValue)
		{
			return true;
		}
	}

	return false;
}

bool TargetManager::IsGoodTarget(Entity* pEntityPtr, int pEntIndex, int_least8_t* pGameTypeId)
{
	if (!pEntityPtr->GetIsPawnInit())
		return false;

	Pawn* lpPawn{ LocalPlayer::GetPawn() };
	Pawn* entityPawn{ pEntityPtr->GetPawnBase() };

	// If this is the localPlayer
	if (lpPawn == entityPawn)
		return false;

	// Entity dead
	if (entityPawn->health < 1)
		return false;

	constexpr int_least8_t deathmatchId{ 0x27 };

	// No Team check when there is no team
	if (*pGameTypeId != deathmatchId && *pGameTypeId != deathmatchId+1)
	{
		if (lpPawn->team_variable == entityPawn->team_variable)
			return false;
	}

	constexpr int_least8_t localPlayerIndex{ 1 };

	// Not spotted by entity AND entity not spotted me
	if (entityPawn->spottedId != localPlayerIndex &&
		lpPawn->spottedId != pEntIndex)
		return false;

	Vector3 targetAngle{ GetTargetAngle(entityPawn->angles) };
	if (!IsTargetInFov(targetAngle))
		return false;

	return true;
}

std::vector<Pawn*> TargetManager::GetTargetsPawn(intptr_t* pEntListBasePtr, int_least8_t* pGameTypeId)
{
	std::vector<Pawn*> targetsPawn{};
	constexpr int paddingBetweenEnt{ 0x78 };

	for (int i{ 1 }; i < 64; ++i)
	{
		Entity currEntity(reinterpret_cast<intptr_t*>(
			reinterpret_cast<intptr_t>(pEntListBasePtr)
			+ i
			* paddingBetweenEnt));

		if (!IsGoodTarget(&currEntity, i, pGameTypeId))
			continue;

		targetsPawn.push_back(currEntity.GetPawnBase());
	}

	return targetsPawn;
}

float TargetManager::NormalizePitch(float pPitch)
{
	pPitch = (pPitch < -89.0f) ? -89.0f : pPitch;

	return (pPitch > 89.f) ? 89.0f : pPitch;
}

float TargetManager::NormalizeYaw(float pYaw)
{
	while (pYaw > 180.f) pYaw -= 360.f;

	while (pYaw < -180.f) pYaw += 360.f;

	return pYaw;
}

float TargetManager::GetMagnitude(const Vector3& pVec)
{
	return ::sqrtf((pVec.x * pVec.x) +
		(pVec.y * pVec.y) +
		(pVec.z * pVec.z));
}

Vector3 TargetManager::GetTargetAngle(Vector3& pTargetPos)
{
	Vector3 targetAngle{ NULL };
	Vector3 lpPos{ LocalPlayer::GetPawn()->body_pos };

	const Vector3 deltaPos{ lpPos - pTargetPos };

	const float magnitudePos{ GetMagnitude(deltaPos) };

	constexpr float radToDegree{ 57.2957795f };

	targetAngle.x = atanf(deltaPos.z / magnitudePos) * radToDegree;
	targetAngle.y = atanf(deltaPos.y / deltaPos.x) * radToDegree;

	if (deltaPos.x >= 0.0f)
		targetAngle.y += 180.0f;

	targetAngle.x = NormalizePitch(targetAngle.x);

	return targetAngle;
}

Pawn* TargetManager::GetNearestTarget(std::vector<Pawn*> pTargetsPawn)
{
	float oldCoef{ FLT_MAX };
	Pawn* lpPawn{ LocalPlayer::GetPawn() };
	Pawn* nearestTarget{ nullptr };

	for (auto currTarget : pTargetsPawn)
	{
		// Get angle distance
		const Vector3 currTargetAngle{ GetTargetAngle(currTarget->body_pos) };
		const Vector3 deltaAngle{ lpPawn->angles - currTargetAngle };
		const float currAngleDist{ GetMagnitude(deltaAngle) };

		// Get body position distance
		const Vector3 deltaPosition{ lpPawn->body_pos - currTarget->body_pos };
		const float currBodyPosDist{ GetMagnitude(deltaPosition) };

		const float currCoef{ currAngleDist * 0.8f + currBodyPosDist * 0.2f };

		if (oldCoef > currCoef)
		{
			oldCoef = currCoef;
			nearestTarget = currTarget;
		}
	}

	return nearestTarget;
}

void TargetManager::SetViewAngleSmooth(Vector3& pTargetAngle, int pSmoothValue)
{
	float* lp_Pitch{ LocalPlayer::GetPitchPtr() };
	float* lp_Yaw{ LocalPlayer::GetYawPtr() };

	Vector3 lp_Angle{ *lp_Pitch, *lp_Yaw, 0 };

	Vector3 deltaAngle{ pTargetAngle - lp_Angle };

	deltaAngle.y = NormalizeYaw(deltaAngle.y);

	if (*lp_Pitch != pTargetAngle.x)
		*lp_Pitch += deltaAngle.x / pSmoothValue;

	if (*lp_Yaw != pTargetAngle.y)
		*lp_Yaw += deltaAngle.y / pSmoothValue;
}