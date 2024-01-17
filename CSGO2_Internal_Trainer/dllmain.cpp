#include "header.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);

    Entity* localPlayer{ EntityManager::GetLocalPlayer()};
    
    intptr_t* entityListBaseAddr{ (intptr_t*)(Offset::clientModBaseAddr + Offset::entitiesList) };

    std::vector<Entity*> entityList{};

    intptr_t* gameStatePtr{ (intptr_t*) (Offset::engineModBaseAddr + Offset::cPredictionBaseAddr + Offset::gameState) };

    constexpr int inGameStateID{ 8 };

    while (!GetAsyncKeyState(VK_DELETE) & 1)
    {
        if (*gameStatePtr == inGameStateID)
        {
         //   std::vector<Entity*> target{ GetTargetList(localPlayer, entityList) };

            //TODO
        //    Entity* nearestTarget{ GetNearestTarget(target) };

            //TODO
            // calculate target angle
      //      GetTargetAngle(nearestTarget);

            //TODO
            // change my angles to aim at target
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

