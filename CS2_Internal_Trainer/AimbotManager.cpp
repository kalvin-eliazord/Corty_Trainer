#include "AimbotManager.h"

bool AimbotManager::Start()
{
	Controller* cTarget{ TargetManager::GetCTarget() };
	if (!cTarget) return false;

	Entity cEntity (cTarget);

	if (!CheatHKeys::bTargetLock)
		CheatHKeys::cBaseLocked = cEntity.GetControllerBase();

	// Target locking option
	if (CheatHKeys::bTargetLock && CheatHKeys::cBaseLocked)
		TargetLockShot();
	else
		TargetShot(cEntity);

	return true;
}

bool AimbotManager::TargetShot(Entity pTarget)
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

bool AimbotManager::TargetLockShot()
{
	Entity targetLocked (CheatHKeys::cBaseLocked);

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
		CheatHKeys::cBaseLocked = nullptr;

	return true;
}
