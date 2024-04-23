#include "TargetManager.h"

bool TargetManager::IsTargetInFov(Vector3& pTargetAngle)
{
	Vector3 localPlayerAngle{ LocalPlayer::GetPawn()->angles };

	Vector3 deltaAngle{};

	if (localPlayerAngle.y > pTargetAngle.y)
		deltaAngle = pTargetAngle - localPlayerAngle;
	else
		deltaAngle = localPlayerAngle - pTargetAngle;

	// Checking if target is in FOV
	if ((deltaAngle.x < CheatHKeys::fovValue) or (-deltaAngle.x > -CheatHKeys::fovValue) &&
		(deltaAngle.y < CheatHKeys::fovValue) or (-deltaAngle.y > -CheatHKeys::fovValue))
	{
		return true;
	}

	return false;
}

bool TargetManager::IsGameDeathMatch(int16_t* pGameTypeId)
{
	constexpr int deathmatchId{ 0x27 };
	constexpr int deathmatchId2{ 0x2B };

	if (*pGameTypeId != deathmatchId &&
		*pGameTypeId != (deathmatchId + 1) &&
		*pGameTypeId != deathmatchId2)
	{
		return false;
	}

	return true;
}

bool TargetManager::IsGoodTarget(Entity* pEntityPtr, int pEntIndex)
{
	if (!pEntityPtr->GetIsPawnInit())
		return false;

	Pawn* lpPawn{ LocalPlayer::GetPawn() };
	if (!lpPawn) return false;

	Pawn* entityPawn{ pEntityPtr->GetPawnBase() };

	// If this is the localPlayer
	if (lpPawn == entityPawn)
		return false;

	// Entity dead
	if (entityPawn->health < 1)
		return false;

	// No Team check when there is no team
	if (!IsGameDeathMatch(Pointer::gameTypeId))
	{
		if (lpPawn->team_variable == entityPawn->team_variable)
			return false;
	}

	if (!ImSpottedAndEntitySpotted(pEntityPtr, pEntIndex))
		return false;

	return true;
}

bool TargetManager::ImSpottedAndEntitySpotted(Entity* pCurrEnt, int pEntIndex)
{
	constexpr int localPlayerId{ 0 };

	std::bitset<64> lpSpottedId{ LocalPlayer::GetEntity().GetSpottedId() };
	std::bitset<64> currEntSpottedId{ pCurrEnt->GetSpottedId() };

	// Not spotted by entity AND entity not spotted me
	if (!(lpSpottedId.test(pEntIndex) || currEntSpottedId.test(localPlayerId)))
		return false;

	return true;
}

Controller* TargetManager::GetNearestCTarget(std::vector<Controller*> pTargetsEnt)
{
	float oldCoef{ FLT_MAX };
	Controller* nearestCTarget{ nullptr };

	Pawn* lpPawn{ LocalPlayer::GetPawn() };

	for (auto currCTarget : pTargetsEnt)
	{
		Entity currEnt(currCTarget);

		// Get angle distance
		const Vector3 currTargetAngle{ GetTargetAngle(currEnt.GetPawnBase()->body_pos) };
		const Vector3 deltaAngle{ lpPawn->angles - currTargetAngle };
		const float angleDistance{ GetMagnitude(deltaAngle) };

		// Get body position distance
		const Vector3 deltaPosition{ lpPawn->body_pos - currEnt.GetPawnBase()->body_pos };
		const float bodyPosDist{ GetMagnitude(deltaPosition) };

		const float currDistCoef{ angleDistance * 0.8f + bodyPosDist * 0.2f };

		if (oldCoef > currDistCoef)
		{
			oldCoef = currDistCoef;
			nearestCTarget = currEnt.GetControllerBase();
		}
	}

	return nearestCTarget;
}

std::vector<Controller*> TargetManager::GetCTargets()
{
	intptr_t* entListBasePtr{ Pointer::entityListBase };
	std::vector<Controller*> cTargetsEntities{};

	for (int i{ 0 }; i < 64; ++i)
	{
		Entity currEntity(*reinterpret_cast<Controller**>(
			reinterpret_cast<intptr_t>(entListBasePtr)
			+ (i + 1)
			* 0x78));

		if (!IsGoodTarget(&currEntity, i))
			continue;

		cTargetsEntities.push_back(currEntity.GetControllerBase());
	}

	return cTargetsEntities;
}

Controller* TargetManager::GetCTarget()
{
	// Get controller targets
	std::vector<Controller*> targetsEnts{ TargetManager::GetCTargets() };
	if (targetsEnts.empty()) return nullptr;

	if (targetsEnts.size() > 1)
		return TargetManager::GetNearestCTarget(targetsEnts);

	return targetsEnts[0];
}

void TargetManager::NormalizePitch(float& pPitch)
{
	pPitch = (pPitch < -89.0f) ? -89.0f : pPitch;

	pPitch = (pPitch > 89.f) ? 89.0f : pPitch;
}

void TargetManager::NormalizeYaw(float& pYaw)
{
	while (pYaw > 180.f) pYaw -= 360.f;

	while (pYaw < -180.f) pYaw += 360.f;
}

float TargetManager::GetMagnitude(const Vector3& pVec)
{
	return ::sqrtf((pVec.x * pVec.x) +
		(pVec.y * pVec.y) +
		(pVec.z * pVec.z));
}
Vector3 TargetManager::GetTargetAngle(Vector3 pTargetPos)
{
	Vector3 targetAngle{ NULL };
	Vector3 lpPos{ LocalPlayer::GetPawn()->body_pos };

	const Vector3 deltaPos{ pTargetPos - lpPos };

	const float distPos{ GetMagnitude(deltaPos) };

	constexpr float radToDegree{ 57.295776f };

	targetAngle.x = -asinf(deltaPos.z / distPos) * radToDegree;
	targetAngle.y = atan2f(deltaPos.y, deltaPos.x) * radToDegree;

	NormalizePitch(targetAngle.x);

	return targetAngle;
}

void TargetManager::SetLpAngleSmooth(Vector3& pTargetAngle, const int pSmoothValue)
{
	float* lp_Pitch{ LocalPlayer::GetPitchPtr() };
	float* lp_Yaw{ LocalPlayer::GetYawPtr() };

	Vector3 lp_Angle{ *lp_Pitch, *lp_Yaw, 0 };

	Vector3 deltaAngle{ pTargetAngle - lp_Angle };

	NormalizeYaw(deltaAngle.y);

	if (*lp_Pitch != pTargetAngle.x)
		*lp_Pitch += deltaAngle.x / pSmoothValue;

	if (*lp_Yaw != pTargetAngle.y)
		*lp_Yaw += deltaAngle.y / pSmoothValue;
}