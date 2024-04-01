#include "CheatManager.h"

bool CheatManager::StartAimbot()
{
	// Initializing a usable console
	ConsoleManager consoleManager{};

	EntityList* entitiesListPtr{ (EntityList*)GamePointer::entityListPtr };
	Entity* localPlayer{ LocalPlayer::Get() };

	GameChecker::gameStateIdPtr = GamePointer::gameStateIdPtr;
	int oldGameStateId{ *GameChecker::gameStateIdPtr };

	// Cheat loop
	while (!GetAsyncKeyState(VK_DELETE) & 1)
	{
		// Aimbot options keybind
		if (GetAsyncKeyState(VK_F4) & 1)
		{
			++AimbotOptions::smoothValue;
			consoleManager.bConsoleChanged = true;
		}
		else if (GetAsyncKeyState(VK_F3) & 1 && AimbotOptions::smoothValue > 0)
		{
			--AimbotOptions::smoothValue;
			consoleManager.bConsoleChanged = true;
		}
		else if (GetAsyncKeyState(VK_F2) & 1)
		{
			AimbotOptions::bTargetLock = !AimbotOptions::bTargetLock;
			consoleManager.bConsoleChanged = true;
		}
		else if (GetAsyncKeyState(VK_F5) & 1 && AimbotOptions::fovValue > 10)
		{
			AimbotOptions::fovValue -= 10;
			consoleManager.bConsoleChanged = true;
		}
		else if (GetAsyncKeyState(VK_F6) & 1)
		{
			AimbotOptions::fovValue += 10;
			consoleManager.bConsoleChanged = true;
		}

		// Checking var that will prevent read access violation errors
		if (*GameChecker::gameStateIdPtr == GameChecker::inGameId && localPlayer && localPlayer->team_variable != 0)
		{
			// IN game
			if (consoleManager.bConsoleChanged or
				*GameChecker::gameStateIdPtr != oldGameStateId or
				consoleManager.bStartingInGame)
			{
				// Print all aimbot options current status
				consoleManager.PrintCheatOptions();

				if (!GamePointer::InitializePointersInGame()) return false;

				GameChecker::gameTypePtr = GamePointer::gameTypeIdPtr;
				oldGameStateId = GameChecker::inGameId;
				consoleManager.bStartingInGame = false;
				consoleManager.bConsoleChanged = false;
			}

			// Retrieve the target list
			std::vector<Entity*> targetList{ entitiesListPtr->GetTargetList(localPlayer, GameChecker::gameTypePtr) };

			if (!targetList.empty())
			{
				// Get Nearest target from lp crosshair
				Entity* nearestTarget{ TargetManager::GetNearestTarget(localPlayer, targetList) };
				Vector3 targetAngle{ TargetManager::GetTargetAngle(localPlayer, nearestTarget) };

				const Vector3 delta_lp_target_angle{ BasicMath::GetDelta(localPlayer->angles, targetAngle) };

				if (AimbotOptions::bTargetLock)
				{
					// If there is a target
					if (AimbotOptions::targetLocked != nullptr)
					{
						Vector3 targetLockedAngle{ TargetManager::GetTargetAngle(localPlayer, AimbotOptions::targetLocked) };

						// If the fov is right you can aim at enemy
						if ((delta_lp_target_angle.x) < AimbotOptions::fovValue or 
							-delta_lp_target_angle.x > -AimbotOptions::fovValue)
						{
							if ((delta_lp_target_angle.y) < AimbotOptions::fovValue or
								-delta_lp_target_angle.y > -AimbotOptions::fovValue)
							{
								if (GetAsyncKeyState(0x02))
								{
									if (AimbotOptions::smoothValue != 0)
										TargetManager::SetViewAngleSmooth(targetAngle, AimbotOptions::smoothValue);
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
					// If the fov is right you can aim at enemy
					if ((delta_lp_target_angle.x) < AimbotOptions::fovValue or -delta_lp_target_angle.x > -AimbotOptions::fovValue)
					{
						if ((delta_lp_target_angle.y) < AimbotOptions::fovValue or -delta_lp_target_angle.y > -AimbotOptions::fovValue)
						{
							if (GetAsyncKeyState(0x02))
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
			// If we launch the prog into the lobby 
			// or if we return in lobby after a game 
			// or if we change the value console
			if (consoleManager.bWaitingLobbyMsg or consoleManager.bConsoleChanged or oldGameStateId != *GameChecker::gameStateIdPtr)
			{
				// Print all aimbot options current status
				consoleManager.PrintCheatOptions();

				oldGameStateId = GameChecker::notInGameId;
				consoleManager.bWaitingLobbyMsg = false;
				consoleManager.bConsoleChanged = false;
			}
			
			if(!localPlayer)
				localPlayer = LocalPlayer::Get();
		}
		Sleep(5);
	}

	return true;
}