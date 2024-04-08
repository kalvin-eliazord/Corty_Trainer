#include "CheatManager.h"

bool CheatManager::StartAimbot()
{
	Entity localPlayer(reinterpret_cast<intptr_t*>(LocalPlayer::Get()));

	if (!GameChecker::oldGameStateId)
		GameChecker::oldGameStateId = *GamePointer::gameStateIdPtr;

	AimbotOptions::OptionsCheck();

	// IN game
	if (*GamePointer::gameStateIdPtr == GameChecker::inGameId)
	{
		if (ConsoleManager::bConsoleChanged or
			*GamePointer::gameStateIdPtr != GameChecker::oldGameStateId or
			ConsoleManager::bInGameStart)
		{
			ConsoleManager::PrintCheatOptions();

			if (!GamePointer::InitializePointersInGame()) return false;

			GameChecker::oldGameStateId = GameChecker::inGameId;
			if(ConsoleManager::bInGameStart) ConsoleManager::bInGameStart = false;
			ConsoleManager::bConsoleChanged = false;
		}

		// Retrieve the target position list
		std::vector<Pawn*> targetsPawn{ TargetManager::GetTargetsPawn(localPlayer.pawnBase, GamePointer::gameTypeIdPtr) };

		if (targetsPawn.empty()) return false;

		// Nearest target from lp crosshair
		Pawn* nearestTarget{ TargetManager::GetNearestTarget(localPlayer.pawnBase, targetsPawn) };
		if (!nearestTarget) return false;

		if (!AimbotOptions::bTargetLock) AimbotOptions::targetLocked = nearestTarget;

		if (AimbotOptions::bTargetLock && AimbotOptions::targetLocked != nullptr)
		{
			Vector3 targetLockedAngle{ TargetManager::GetTargetAngle(localPlayer.pawnBase->body_pos, AimbotOptions::targetLocked->body_pos) };

			if (AimbotOptions::IsTargetInFov(localPlayer.pawnBase->angles, targetLockedAngle))
			{
				if (GetAsyncKeyState(VK_RBUTTON))
				{
					if (AimbotOptions::smoothValue != 0)
						TargetManager::SetViewAngleSmooth(targetLockedAngle, AimbotOptions::smoothValue);
					else
						LocalPlayer::SetViewAngle(targetLockedAngle);
				}
			}

			// Locking at enemy until he die
			if (AimbotOptions::targetLocked->health < 1)
				AimbotOptions::targetLocked = nullptr;
		}
		else
		{
			Vector3 targetAngle{ TargetManager::GetTargetAngle(localPlayer.pawnBase->body_pos, nearestTarget->body_pos) };

			if (AimbotOptions::IsTargetInFov(localPlayer.pawnBase->angles, targetAngle))
			{
				if (GetAsyncKeyState(VK_RBUTTON))
				{
					if (AimbotOptions::smoothValue != 0)
						TargetManager::SetViewAngleSmooth(targetAngle, AimbotOptions::smoothValue);
					else
						LocalPlayer::SetViewAngle(targetAngle);
				}
			}
		}
	}
	// NOT in game
	else
	{
		if (ConsoleManager::bLobbyStart or ConsoleManager::bConsoleChanged or GameChecker::oldGameStateId != *GamePointer::gameStateIdPtr)
		{
			ConsoleManager::PrintCheatOptions();

			GameChecker::oldGameStateId = GameChecker::notInGameId;
			ConsoleManager::bLobbyStart = false;
			ConsoleManager::bConsoleChanged = false;
		}
	}

	Sleep(5);

	return true;
}