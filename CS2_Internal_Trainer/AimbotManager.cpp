#include "AimbotManager.h"

bool AimbotManager::Start()
{
	Controller* cTarget{ TargetManager::GetCTarget() };
	if (!cTarget) return false;

	Entity cEntity (cTarget);

	if (!CheatOptions::bTargetLock)
		CheatOptions::cBaseLocked = cEntity.GetControllerBase();

	// Target locking option
	if (CheatOptions::bTargetLock && CheatOptions::cBaseLocked)
		TargetLockShot();
	else
		TargetShot(cEntity);

	return true;
}

bool AimbotManager::TargetShot(Entity pTarget)
{
	Vector3 targetAngle{ TargetManager::GetTargetAngle(pTarget.GetHeadPos()) };
	if (!TargetManager::IsTargetInFov(targetAngle)) return false;

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		if (CheatOptions::smoothValue)
			TargetManager::SetViewAngleSmooth(targetAngle, CheatOptions::smoothValue);
		else
			LocalPlayer::SetViewAngle(targetAngle);
	}

	return true;
}

bool AimbotManager::TargetLockShot()
{
	Entity cTargetLocked (CheatOptions::cBaseLocked);

	Vector3 targetLockedAngle{ TargetManager::GetTargetAngle(cTargetLocked.GetHeadPos()) };

	if (!TargetManager::IsTargetInFov(targetLockedAngle)) return false;

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		if (CheatOptions::smoothValue)
			TargetManager::SetViewAngleSmooth(targetLockedAngle, CheatOptions::smoothValue);
		else
			LocalPlayer::SetViewAngle(targetLockedAngle);
	}

	// Locking at target until he die
	if (cTargetLocked.GetPawnBase()->health < 1)
		CheatOptions::cBaseLocked = nullptr;

	return true;
}
