#include "AimbotManager.h"

bool AimbotManager::Start()
{
	Controller* cTarget{ TargetManager::GetCTarget() };
	if (!cTarget) return false;

	Entity cEntity (cTarget);

	if (!CheatHKeys::bTargetLock)
		TargetManager::cTargetLocked = cEntity.GetControllerBase();

	// Target locking option
	if (CheatHKeys::bTargetLock && TargetManager::cTargetLocked)
		ShotLockedTarget();
	else
		ShotTarget(cEntity);

	return true;
}

bool AimbotManager::ShotTarget(Entity pTarget)
{
	Vector3 targetAngle{};

	if (CheatHKeys::bHeadPos)
		targetAngle = TargetManager::GetTargetAngle(pTarget.GetHeadPos());
	else
		targetAngle = TargetManager::GetTargetAngle(pTarget.GetPelvisPos());

	if (!TargetManager::IsTargetInFov(targetAngle)) return false;

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		if (CheatHKeys::smoothValue)
			TargetManager::SetViewAngleSmooth(targetAngle, CheatHKeys::smoothValue);
		else
			LocalPlayer::SetViewAngle(targetAngle);
	}

	return true;
}

bool AimbotManager::ShotLockedTarget()
{
	Entity targetLocked (TargetManager::cTargetLocked);

	Vector3 targetLockedAngle{};

	if (CheatHKeys::bHeadPos)
		targetLockedAngle = TargetManager::GetTargetAngle(targetLocked.GetHeadPos());
	else
		targetLockedAngle = TargetManager::GetTargetAngle(targetLocked.GetPelvisPos());

	if (!TargetManager::IsTargetInFov(targetLockedAngle)) return false;

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		if (CheatHKeys::smoothValue)
			TargetManager::SetViewAngleSmooth(targetLockedAngle, CheatHKeys::smoothValue);
		else
			LocalPlayer::SetViewAngle(targetLockedAngle);
	}

	// Locking at target until he die
	if (targetLocked.GetPawnBase()->health < 1)
		TargetManager::cTargetLocked = nullptr;

	return true;
}