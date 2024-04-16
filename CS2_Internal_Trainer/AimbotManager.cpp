#include "AimbotManager.h"

bool AimbotManager::Start()
{
	Pawn* lpPawn(LocalPlayer::GetPawn());

	Controller* cTarget{ TargetManager::GetCTarget() };
	if (!cTarget) return false;

	Entity cEntity (cTarget);

	if (!CheatOptions::bTargetLock)
		CheatOptions::entLocked = &cEntity;

	// Target locking option
	if (CheatOptions::bTargetLock && CheatOptions::entLocked)
		TargetLockShot();
	else
		TargetShot(&cEntity);

	return true;
}

bool AimbotManager::TargetShot(Entity* pTarget)
{
	Vector3 targetAngle{ TargetManager::GetTargetAngle(pTarget->GetHeadPos()) };
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
	Vector3 targetLockedAngle{ TargetManager::GetTargetAngle(CheatOptions::entLocked->GetHeadPos()) };
	if (!TargetManager::IsTargetInFov(targetLockedAngle)) return false;

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		if (CheatOptions::smoothValue)
			TargetManager::SetViewAngleSmooth(targetLockedAngle, CheatOptions::smoothValue);
		else
			LocalPlayer::SetViewAngle(targetLockedAngle);
	}

	// Locking at target until he die
	if (CheatOptions::entLocked->GetPawnBase()->health < 1)
		CheatOptions::entLocked = nullptr;

	return true;
}
