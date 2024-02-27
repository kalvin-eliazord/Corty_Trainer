#include "header.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
    // Initialize a console
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);
    
    // Initialize game logic address
    Entity* localPlayer{ (Entity*) LocalPlayer::Get()};
    EntityList* entitiesList{ (EntityList*)GameOffset::Client::entitiesListPtr };
    
    while (!GetAsyncKeyState(VK_DELETE) & 1)
    {
        // Initialize a variable that will prevent read access violation errors
        if (*GameChecker::gameStateIdPtr == GameChecker::inGameId &&
            entitiesList->GetNbEntAlive() != NULL)
        {
            std::cout << "IN GAME, HAVE FUN. \t \r";

            // Retrieve the target list
            std::vector<Entity*> targetList{ entitiesList->GetTargetList(localPlayer) };

            if (!targetList.empty())
            {
                Entity* nearestTarget{ TargetManager::GetNearestTarget(localPlayer, targetList) };

                const Vector3 targetAngle{ TargetManager::GetTargetAngle(localPlayer, nearestTarget) };

                // Right click to set lp angle
                if (GetAsyncKeyState(0x02))
                    LocalPlayer::SetViewAngle(targetAngle);
            }
        }
        // NOT in game
        else
        {
            std::cout << "LAUNCH A GAME.            \t \r";
            Sleep(5);
        }
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
        HANDLE hThread{ CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, nullptr)};

        if (hThread)
            CloseHandle(hThread);
    }
    return TRUE;
}