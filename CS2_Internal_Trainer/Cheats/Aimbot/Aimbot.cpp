#include "Aimbot.h"

bool Aimbot::IsTargetInFov(Vector3& pTargetAngle)
{
	Vector3 localPlayerAngle{ LocalPlayer::GetPawn().vAngEyeAngle };

	Vector3 deltaAngle{ localPlayerAngle - pTargetAngle };

	const float distFromCursor{ GetMagnitude(deltaAngle) };

	// Checking if target is in FOV
	if (distFromCursor < CheatHKeys::fovValue)
		return true;

	return false;
}

bool Aimbot::IsGoodTarget(Entity* pEntityPtr, int pEntIndex)
{
	if (!pEntityPtr->IsEntInit())
		return false;

	Pawn lpPawn{ LocalPlayer::GetPawn() };
	Pawn entityPawn{ pEntityPtr->GetPawnBase() };

	if (LocalPlayer::GetController().sEntName == pEntityPtr->GetCBase().sEntName)
		return false;

	if (entityPawn.iHealth < 1)
		return false;

	if (CheatHKeys::bTeamCheck)
	{
		if (lpPawn.iTeamNum == entityPawn.iTeamNum)
			return false;
	}

	if (!IsSpotted(pEntityPtr, pEntIndex))
		return false;

	return true;
}

bool Aimbot::IsSpotted(Entity* pCurrEnt, int pEntIndex)
{
	constexpr int localPlayerId{ 0 };

	std::bitset<64> lpSpottedId{ LocalPlayer::GetEntity().GetPawnBase().bSpottedMask };
	std::bitset<64> currEntSpottedId{ pCurrEnt->GetPawnBase().bSpottedMask };

	// Not spotted by entity AND entity not spotted me
	if (!(lpSpottedId.test(pEntIndex) || currEntSpottedId.test(localPlayerId)))
		return false;

	return true;
}

Entity Aimbot::GetNearestTarget(std::vector<Entity> pTargetsEnt)
{
	float oldCoef{ FLT_MAX };
	Entity nearestTarget{};

	Pawn lpPawn{ LocalPlayer::GetPawn() };

	for (const auto& currCTarget : pTargetsEnt)
	{
		Entity currEnt(currCTarget);

		// Get angle distance
		const Vector3 currTargetAngle{ GetTargetAngle(currEnt.GetPawnBase().vLastCameraPos) };
		const Vector3 deltaAngle{ lpPawn.vAngEyeAngle - currTargetAngle };
		const float angleDistance{ GetMagnitude(deltaAngle) };

		// Get body position distance
		const Vector3 deltaPosition{ lpPawn.vLastCameraPos - currEnt.GetPawnBase().vLastCameraPos };
		const float bodyPosDist{ GetMagnitude(deltaPosition) };

		const float currDistCoef{ angleDistance * 0.8f + bodyPosDist * 0.2f };

		if (oldCoef > currDistCoef)
		{
			oldCoef = currDistCoef;
			nearestTarget = currEnt;
		}
	}

	return nearestTarget;
}

std::vector<Entity> Aimbot::GetValidTargets()
{
	std::vector<Entity> cTargetsEntities{};

	for (int i{ 0 }; i < 64; ++i)
	{
		Entity currEntity(MyPointers::GetEntityBase(i));

		if (!IsGoodTarget(&currEntity, i))
			continue;

		cTargetsEntities.push_back(currEntity);
	}

	return cTargetsEntities;
}

bool Aimbot::GetEntTarget(Entity& pEntityTarget)
{
	// Get Entities targets
	std::vector<Entity> targetsEntities{ Aimbot::GetValidTargets() };
	if (targetsEntities.empty()) return false;

	if (targetsEntities.size() > 1)
		pEntityTarget = GetNearestTarget(targetsEntities);
	else
		pEntityTarget = targetsEntities[0];

	return true;
}

void Aimbot::NormalizePitch(float& pPitch)
{
	pPitch = (pPitch < -89.0f) ? -89.0f : pPitch;

	pPitch = (pPitch > 89.f) ? 89.0f : pPitch;
}

void Aimbot::NormalizeYaw(float& pYaw)
{
	while (pYaw > 180.f) pYaw -= 360.f;

	while (pYaw < -180.f) pYaw += 360.f;
}

float Aimbot::GetMagnitude(const Vector3& pVec)
{
	return ::sqrtf((pVec.x * pVec.x) +
		(pVec.y * pVec.y) +
		(pVec.z * pVec.z));
}
Vector3 Aimbot::GetTargetAngle(Vector3 pTargetPos)
{
	Vector3 targetAngle{ NULL };
	Vector3 lpPos{ LocalPlayer::GetPawn().vLastCameraPos };

	const Vector3 deltaPos{ pTargetPos - lpPos };

	const float distPos{ GetMagnitude(deltaPos) };

	constexpr float radToDegree{ 57.295776f };

	targetAngle.x = -asinf(deltaPos.z / distPos) * radToDegree;
	targetAngle.y = atan2f(deltaPos.y, deltaPos.x) * radToDegree;

	NormalizePitch(targetAngle.x);

	return targetAngle;
}

bool Aimbot::Start()
{
	Entity cTarget;
	GetEntTarget(cTarget);

	if (!cTarget.IsEntInit()) return false;

	// Updating the target only when the feature is off
	if (!CheatHKeys::bTargetLock)
		cTargetLocked = cTarget;

	// Target locking feature
	if (CheatHKeys::bTargetLock)
		ShotLockedTarget();
	else
		ShotTarget(cTarget);

	return true;
}

bool Aimbot::ShotTarget(const Entity& pCTarget)
{
	Entity entTarget(pCTarget);

	Pawn entPawn{ entTarget.GetPawnBase() };

	Vector3 targetAngle{};

	targetAngle = Aimbot::GetTargetAngle(entPawn.vLastCameraPos);

	 if (CheatHKeys::bHeadPos)
	 	targetAngle = Aimbot::GetTargetAngle(entPawn.headBonePos);
	 else
	 	targetAngle = Aimbot::GetTargetAngle(entPawn.pelvisBonePos);

	if (!Aimbot::IsTargetInFov(targetAngle))
		return false;

	if (CheatHKeys::smoothValue)
		LocalPlayer::SetSmoothViewAngles(targetAngle, CheatHKeys::smoothValue);
	else
		LocalPlayer::SetViewAngles(targetAngle);

	return true;
}

bool Aimbot::ShotLockedTarget()
{
	Entity entTargetLocked(Aimbot::cTargetLocked);

	Pawn entPawnLocked{ entTargetLocked.GetPawnBase() };

	Vector3 targetLockedAngle{};

	targetLockedAngle = Aimbot::GetTargetAngle(entPawnLocked.vLastCameraPos);

	 if (CheatHKeys::bHeadPos)
	 	targetLockedAngle = Aimbot::GetTargetAngle(entPawnLocked.headBonePos);
	 else
	 	targetLockedAngle = Aimbot::GetTargetAngle(entPawnLocked.pelvisBonePos);

	if (!Aimbot::IsTargetInFov(targetLockedAngle)) 
		return false;

	if (CheatHKeys::smoothValue)
		LocalPlayer::SetSmoothViewAngles(targetLockedAngle, CheatHKeys::smoothValue);
	else
		LocalPlayer::SetViewAngles(targetLockedAngle);

	// Locking at target until he die
	if (entPawnLocked.iHealth < 1)
		cTargetLocked = NULL;

	return true;
}