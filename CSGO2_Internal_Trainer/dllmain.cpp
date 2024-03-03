#include "header.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
    // Initialize a console
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);
    
    // Initialize game logic address
    Entity* localPlayer{ LocalPlayer::Get()};
    EntityList* entitiesListPtr{ (EntityList*)GameOffset::Client::entitiesListBaseAddr };
    int nbEntAlive{ 1 };

    // Aimbot options
    bool bTargetLock{ false };
    int smoothValue{ 0 };

    // Console updater
    bool bConsoleChanged{ false };
    bool bWaitingLobbyMsg{ true };
    bool bStartingInGame{ true };
    int oldGameStateId{ *GameChecker::gameStateIdPtr };

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

        // Initialize a variable that will prevent read access violation errors
        if (*GameChecker::gameStateIdPtr == GameChecker::inGameId &&
            entitiesListPtr->GetNbEntAlive() > 1 && localPlayer)
        {
            if (bConsoleChanged or
                *GameChecker::gameStateIdPtr != oldGameStateId or
                 bStartingInGame)
            {
                // Aimbot options status
                system("cls");
                std::cout << "IN GAME, HAVE FUN. \n";
                std::cout << "------------------- \n";
                std::cout << "[[SMOOTH_VALUE]]-->" << "[[" << smoothValue << "]]" << "\n";
                std::cout << "[[TARGET_LOCK ]]-->" << std::boolalpha << "[[" << bTargetLock << "]] " << "\n";
                
                oldGameStateId = GameChecker::inGameId;
                bConsoleChanged = false;
                bStartingInGame = false;
            }

            // Retrieve the target list
            std::vector<Entity*> targetList{ entitiesListPtr->GetTargetList(localPlayer, nbEntAlive) };

            if (!targetList.empty())
            {
                // Nearest target from lp crosshair
                Entity* nearestTarget{ TargetManager::GetNearestTarget(localPlayer, targetList) };
                Vector3 targetAngle{ TargetManager::GetTargetAngle(localPlayer, nearestTarget) };

                // Locking at enemy until he dead or the option is turnt off
                if (bTargetLock)
                {
                    while (nearestTarget->health > 0 or !GetAsyncKeyState(VK_F2)&1 or !GetAsyncKeyState(VK_DELETE) & 1)
                    {
                        Vector3 targetAngleLocked{ TargetManager::GetTargetAngle(localPlayer, nearestTarget) };

                        // Right click to set lp angle
                        if (GetAsyncKeyState(0x02))
                            LocalPlayer::SetViewAngle(localPlayer->angles, targetAngle, smoothValue);
                    }
                }
                else
                {
                    // Right click to set lp angle
                    if (GetAsyncKeyState(0x02))
                        LocalPlayer::SetViewAngle(localPlayer->angles, targetAngle, smoothValue);
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
                std::cout << "[[SMOOTH_VALUE]]-->" << "[[" << smoothValue << "]]" << "\n";
                std::cout << "[[TARGET_LOCK ]]-->" << std::boolalpha << "[[" << bTargetLock << "]] " << "\n";

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