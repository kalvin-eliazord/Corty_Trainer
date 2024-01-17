#include "header.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);

    Entity* localPlayer{ EntityManager::GetLocalPlayer()};
    
    intptr_t* entityListBaseAddr{ (intptr_t*)(Offset::Client::modBaseAddr + Offset::Client::entitiesList) };
    EntityList* entitiesList{ (EntityList*)entityListBaseAddr };
    std::vector<Entity*> entityList{};

    // To prevent read access violation errors.
    intptr_t* gameStateIdPtr{Offset::Engine2::gameStateID};
    constexpr int inGameStateID{ 8 };


    while (!GetAsyncKeyState(VK_DELETE) & 1)
    {
        if (*gameStateIdPtr == inGameStateID)
        {
            const int allEntAliveNb{ entitiesList->GetNbEntAlive()};

            //test
            std::cout << allEntAliveNb << "\t \r";

           // for (int i{ 0 }; i < (allEntAliveNb * 2); ++i)
            //{

            //}
         //   std::vector<Entity*> target{ GetTargetList(localPlayer, entityList) };

            //TODO
        //    Entity* nearestTarget{ GetNearestTarget(target) };

            //TODO
            // calculate target angle
      //      GetTargetAngle(nearestTarget);

            //TODO
            // change my angles to aim at target
        }
        else
        {
            std::cout << "NOT IN GAME \t \r";
        }
        
        Sleep(5);
    }

    if (f)
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

