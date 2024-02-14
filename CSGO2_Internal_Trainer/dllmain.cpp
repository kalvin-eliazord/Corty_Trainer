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

    // Initialize game logic address
    Entity* localPlayer{ LocalPlayer::Get()};
    EntityList* entitiesList{ (EntityList*)GameOffset::Client::entitiesListPtr };

    // Initialize a variable that will prevent read access violation errors
    int* gameStateId{ (int*) (GameOffset::Client::gameStateIdPtr)};
    constexpr int inGame_GameStateId{ 8 };

    // Waiting for the game to start loop
    std::cout << "NOT IN GAME, WAITING. \t \r";
    do
    {
        if (GetAsyncKeyState(VK_DELETE) & 1)
            CloseProgram(f, hModule);

        if (*gameStateId == inGame_GameStateId)
        {
            Sleep(2000);
            entitiesList->SetNbEntAlive();
        }
        Sleep(5);
    }
    while (!entitiesList->GetNbEntAlive());

    // Hack loop
    while (!GetAsyncKeyState(VK_DELETE) & 1)
    {
        // In game loop
        if (*gameStateId == inGame_GameStateId)
        {
            std::cout << "IN GAME, HAVE FUN. \t \r";

            // Retrieve the target list
            std::vector<Entity*> targetList{ entitiesList->GetTargetList() };

            if (!targetList.empty())
            {
                // To retrieve the target in the list
                int nearestTargetIndex{ -1 };

                TargetManager targetManager{};

                // Filter the nearest target from local player
                for (int i{ 0 }; i < targetList.size(); ++i)
                {
                    targetManager.SetNearestTarget(targetList[i], i);
                    nearestTargetIndex = targetManager.GetNearestTargetIndex();
                }

                if (nearestTargetIndex != -1)
                {
                    // calculate target angle
                    Vector3 targetAngles = targetManager.GetTargetAngle(targetList[nearestTargetIndex]);

                    float* lpPitch{ (float*)(GameOffset::Client::lp_Pitch_Input) };
                    float* lpYaw{ (float*)(GameOffset::Client::lp_Yaw_Input) };

                    *lpPitch = targetAngles.x;
                    *lpYaw   = targetAngles.y;
                }
            }
           
        }
        // NOT in game loop
        else
        {
            system("CLS");
            std::cout << "NOT IN GAME, WAITING. \t \r";
            entitiesList->nbEntities = NULL;

            do
            {
                if (*gameStateId == inGame_GameStateId)
                {
                    Sleep(2000);
                    entitiesList->SetNbEntAlive();
                }

                Sleep(5);
            } while (!entitiesList->GetNbEntAlive());
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