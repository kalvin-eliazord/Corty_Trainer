#include "CheatManager.h"

bool CheatManager::StartAimbot()
{
	EntityList* entitiesListPtr{ (EntityList*)GamePointer::entityListBasePtr };
	Entity* localPlayer{ LocalPlayer::Get() };

	if (!GameChecker::oldGameStateId)
		GameChecker::oldGameStateId = *GamePointer::gameStateIdPtr;

	AimbotOptions::OptionsCheck();

	// Checking var that will prevent read access violation errors
	if (*GamePointer::gameStateIdPtr == GameChecker::inGameId && localPlayer->team_variable != 0)
	{
		// IN game
		if (ConsoleManager::bConsoleChanged or
			*GamePointer::gameStateIdPtr != GameChecker::oldGameStateId or
			ConsoleManager::bInGameStart)
		{
			// Print all aimbot options current status
			ConsoleManager::PrintCheatOptions();

			if (!GamePointer::InitializePointersInGame()) return false;

			GameChecker::oldGameStateId = GameChecker::inGameId;
			ConsoleManager::bInGameStart = false;
			ConsoleManager::bConsoleChanged = false;
		}

		// Retrieve the target list
		std::vector<Entity*> targetList{ entitiesListPtr->GetTargetList(localPlayer, GamePointer::gameTypeIdPtr) };

		if (!targetList.empty())
		{
			// Get Nearest target from lp crosshair
			Entity* nearestTarget{ TargetManager::GetNearestTarget(localPlayer, targetList) };
			Vector3 targetAngle{ TargetManager::GetTargetAngle(localPlayer, nearestTarget) };

			const Vector3 delta_lp_target_angle{ localPlayer->angles - targetAngle };

			if (AimbotOptions::bTargetLock)
			{
				// If there is a target
				if (AimbotOptions::targetLocked != nullptr)
				{
					Vector3 targetLockedAngle{ TargetManager::GetTargetAngle(localPlayer, AimbotOptions::targetLocked) };

					// Checking if target is in FOV
					if ((delta_lp_target_angle.x) < AimbotOptions::fovValue or
						-delta_lp_target_angle.x > -AimbotOptions::fovValue)
					{
						if ((delta_lp_target_angle.y) < AimbotOptions::fovValue or
							-delta_lp_target_angle.y > -AimbotOptions::fovValue)
						{
							if (GetAsyncKeyState(VK_RBUTTON))
							{
								if (AimbotOptions::smoothValue != 0)
									TargetManager::SetViewAngleSmooth(targetLockedAngle, AimbotOptions::smoothValue);
								else
									LocalPlayer::SetViewAngle(targetLockedAngle);
							}
						}
					}
					// Locking at enemy until he die
					if (AimbotOptions::targetLocked->health < 1)
						AimbotOptions::targetLocked = nullptr;
				}
				else
				{
					AimbotOptions::targetLocked = nearestTarget;
				}
			}
			else
			{
				// Checking if target is in FOV
				if ((delta_lp_target_angle.x) < AimbotOptions::fovValue or -delta_lp_target_angle.x > -AimbotOptions::fovValue)
				{
					if ((delta_lp_target_angle.y) < AimbotOptions::fovValue or -delta_lp_target_angle.y > -AimbotOptions::fovValue)
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
		}
	}
	// NOT in game
	else
	{
		if (ConsoleManager::bLobbyStart or ConsoleManager::bConsoleChanged or GameChecker::oldGameStateId != *GamePointer::gameStateIdPtr)
		{
			// Print all aimbot options current status
			ConsoleManager::PrintCheatOptions();

			GameChecker::oldGameStateId = GameChecker::notInGameId;
			ConsoleManager::bLobbyStart = false;
			ConsoleManager::bConsoleChanged = false;
		}

		if (!localPlayer)
			localPlayer = LocalPlayer::Get();
	}

	Sleep(5);

	return true;
}