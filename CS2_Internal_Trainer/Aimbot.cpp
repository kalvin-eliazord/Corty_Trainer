#include "Aimbot.h"

bool Aimbot::Start()
{
	Pawn* lpPawn(LocalPlayer::GetPawn());
	intptr_t* entityListBasePtr{ GamePointer::entityListBasePtr };

	// Get targets
	std::vector<Pawn*> targetsPawn{ TargetManager::GetTargetsPawn(entityListBasePtr, GamePointer::gameTypeIdPtr) };

	if (targetsPawn.empty()) return false;

	Pawn* nearestTarget{ TargetManager::GetNearestTarget(targetsPawn) };
	if (!nearestTarget) return false;

	if (!AimbotOptions::bTargetLock)
		AimbotOptions::targetLocked = nearestTarget;

	// Target locking option
	if (AimbotOptions::bTargetLock && AimbotOptions::targetLocked)
	{
		Vector3 targetLockedAngle{ TargetManager::GetTargetAngle(AimbotOptions::targetLocked->body_pos) };

		if (AimbotOptions::IsTargetInFov(lpPawn->angles, targetLockedAngle))
		{
			if (GetAsyncKeyState(VK_RBUTTON))
			{
				if (AimbotOptions::smoothValue)
					TargetManager::SetViewAngleSmooth(targetLockedAngle, AimbotOptions::smoothValue);
				else
					LocalPlayer::SetViewAngle(targetLockedAngle);
			}
		}

		// Locking at target until he die
		if (AimbotOptions::targetLocked->health < 1)
			AimbotOptions::targetLocked = nullptr;
	}
	else
	{
		// Default target option
		Vector3 targetAngle{ TargetManager::GetTargetAngle(nearestTarget->body_pos) };

		if (AimbotOptions::IsTargetInFov(lpPawn->angles, targetAngle))
		{
			if (GetAsyncKeyState(VK_RBUTTON))
			{
				if (AimbotOptions::smoothValue)
					TargetManager::SetViewAngleSmooth(targetAngle, AimbotOptions::smoothValue);
				else
					LocalPlayer::SetViewAngle(targetAngle);
			}
		}
	}

	return true;
}
