#include "header.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
    if (!GameOffset::UpdatePointers()) return 0;

    EntityList* entitiesListPtr{ GameOffset::Client::entityListPtr };
    Entity* localPlayer{ GameOffset::Client::localPlayerPtr };

    GameChecker::gameStateIdPtr = GameOffset::Client::gameStateIdPtr;
    GameChecker::gameTypePtr = GameOffset::Client::gameTypeIdPtr;

    int oldGameStateId{ *GameChecker::gameStateIdPtr };

    // Initialize a console
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);

   // Aimbot options
    Entity* targetLocked{ nullptr };
    bool bTargetLock{ false };
    int smoothValue{ 0 };
    int fovValue{ 50 };

    // Console updater
    bool bConsoleChanged{ false };
    bool bWaitingLobbyMsg{ true };
    bool bStartingInGame{ true };
    
    while (!GetAsyncKeyState(VK_DELETE) & 1)
    {
        // Aimbot options keybind
        if (GetAsyncKeyState(VK_F4) & 1)
        {
            ++smoothValue;
            bConsoleChanged = true;
        }
        else if (GetAsyncKeyState(VK_F3) & 1 && smoothValue > 0)
        {
            --smoothValue;
            bConsoleChanged = true;
        }

        if (GetAsyncKeyState(VK_F2) & 1)
        {
            bTargetLock = !bTargetLock;
            bConsoleChanged = true;
        }

        if (GetAsyncKeyState(VK_F5) & 1 && fovValue > 10)
        {
            fovValue -= 10;
            bConsoleChanged = true;
        }

        if (GetAsyncKeyState(VK_F6) & 1)
        {
            fovValue += 10;
            bConsoleChanged = true;
        }

        // Initialize a variable that will prevent read access violation errors
        if (*GameChecker::gameStateIdPtr == GameChecker::inGameId &&
            localPlayer && localPlayer->team_variable != 0  )
        {
            if (bConsoleChanged or
                *GameChecker::gameStateIdPtr != oldGameStateId or
                 bStartingInGame)
            {
                // Aimbot options status
                system("cls");
                std::cout << "IN GAME, HAVE FUN. \n";
                std::cout << "------------------- \n";
                std::cout << "SMOOTH VALUE: press F3 (-) or press F4 (+) \n" << "-->[[" << smoothValue << "]]" << "\n";
                std::cout << "FOV: press F5 (-) or press F6 (+) \n" << "-->[[" << fovValue << "]]" << "\n";
                std::cout << "TARGET LOCKING: press F2 \n" << std::boolalpha << "-->[[" << bTargetLock << "]] " << "\n";
                
                oldGameStateId = GameChecker::inGameId;
                bConsoleChanged = false;
                bStartingInGame = false;
            }

            // Retrieve the target list
             std::vector<Entity*> targetList{ entitiesListPtr->GetTargetList(localPlayer, GameChecker::gameTypePtr) };

            if (!targetList.empty())
            {
                // Get Nearest target from lp crosshair
                Entity* nearestTarget{ TargetManager::GetNearestTarget(localPlayer, targetList) };
                Vector3 targetAngle{ TargetManager::GetTargetAngle(localPlayer, nearestTarget) };

                const Vector3 delta_lp_target_angle{ BasicMath::GetDelta(localPlayer->angles, targetAngle) };

                if (bTargetLock)
                {
                    if (targetLocked)
                    {
                        Vector3 targetLockedAngle{ TargetManager::GetTargetAngle(localPlayer, targetLocked) };

                        // If the fov is right you can aim at enemy
                        if ((delta_lp_target_angle.x) < fovValue or -delta_lp_target_angle.x > -fovValue)
                        {
                            if ((delta_lp_target_angle.y) < fovValue or -delta_lp_target_angle.y > -fovValue)
                            {
                                if (GetAsyncKeyState(0x02))
                                    LocalPlayer::SetViewAngle(targetLockedAngle, smoothValue);
                            }
                        }

                        // Locking at enemy until he dead
                        if (targetLocked->health < 1)
                            targetLocked = nullptr;
                    }
                    else
                    {
                        targetLocked = nearestTarget;
                    }
                }
                else
                {
                    // If the fov is right you can aim at enemy
                    if ((delta_lp_target_angle.x) < fovValue or -delta_lp_target_angle.x > -fovValue)
                    {
                        if ((delta_lp_target_angle.y) < fovValue or -delta_lp_target_angle.y > -fovValue)
                        {
                            if (GetAsyncKeyState(0x02))
                                LocalPlayer::SetViewAngle(targetAngle, smoothValue);
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
            if (bWaitingLobbyMsg or 
                bConsoleChanged or 
                oldGameStateId != *GameChecker::gameStateIdPtr)
            {
                system("cls");
                std::cout << "WAITING FOR A GAME. \n";
                std::cout << "------------------- \n";
                std::cout << "SMOOTH VALUE: press F3 (-) or press F4 (+) \n" << "-->[[" << smoothValue << "]]" << "\n";
                std::cout << "FOV: press F5 (-) or press F6 (+) \n" << "-->[[" << fovValue << "]]" << "\n";
                std::cout << "TARGET LOCKING: press F2 \n" << std::boolalpha << "-->[[" << bTargetLock << "]] " << "\n";

                bWaitingLobbyMsg = false;
                bConsoleChanged = false;
                oldGameStateId = GameChecker::notInGameId;
            }

            localPlayer = LocalPlayer::Get();
        }
        Sleep(5);
    }
    
    if(f)
        fclose(f);

    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        HANDLE hMainThread{ CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, nullptr)};

        if (hMainThread)
            CloseHandle(hMainThread);
    }
    return TRUE;
}