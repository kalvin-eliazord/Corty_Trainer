#include "header.h"

void CloseProgram(FILE* f, HMODULE hModule)
{
    if (f)
        fclose(f);

    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

DWORD WINAPI MainThread(HMODULE hModule)
{
    // Initialize a console
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "WAITING FOR A GAME.";
    
    // Initialize game logic address
    Entity* localPlayer{ LocalPlayer::Get()};
    EntityList* entitiesList{ (EntityList*)GameOffset::Client::entitiesListPtr };

    // Before a game loop
    do
    {
        if (GetAsyncKeyState(VK_DELETE) & 1)
            CloseProgram(f, hModule);

        GameChecker::CheckGameState(entitiesList);
        Sleep(5);
    }
    while (!entitiesList->GetNbEntAlive());
    
    while (!GetAsyncKeyState(VK_DELETE) & 1)
    {
        // Initialize a variable that will prevent read access violation errors
        if (*GameChecker::gameStateIdPtr == GameChecker::inGameId)
        {
            std::cout << "IN GAME, HAVE FUN. \t \r";

            // Retrieve the target list
            std::vector<Entity*> targetList{ entitiesList->GetTargetList() };

            if (!targetList.empty())
            {
                // To retrieve the target in the list
                int nearestTargetIndex{ -1 };

                TargetManager targetManager{};

                // Iterate each valid target
                for (int i{ 0 }; i < targetList.size(); ++i)
                {
                    // calculate target angle
                    Vector3 targetAngles = targetManager.GetTargetAngle(targetList[nearestTargetIndex]);

                    targetManager.SetNearestTarget(targetList[i], i);
                    nearestTargetIndex = targetManager.GetNearestTargetIndex();

     
                }

                if (nearestTargetIndex != -1)
                {


                    // Right click to set lp angle
                    if(GetAsyncKeyState(0x02))
                        LocalPlayer::SetViewAngle(targetAngles.x, targetAngles.y);
                }
            }
        }
        // NOT in game
        else
        {
            if (GetAsyncKeyState(VK_DELETE) & 1)
                CloseProgram(f, hModule);

            // Cleaning nb entities alive after a game
            entitiesList->SetNbEntAlive(NULL);

            system("CLS");
            std::cout << "WAITING FOR A GAME.";

            // Before a game loop
            do
            {
                GameChecker::CheckGameState(entitiesList);
                Sleep(5);
            }
            while (!entitiesList->GetNbEntAlive());
        }
        
        Sleep(5);
    }

    CloseProgram(f, hModule);
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