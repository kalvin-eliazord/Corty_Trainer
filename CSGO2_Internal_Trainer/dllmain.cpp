#include "header.h"

bool IsGoodTarget(Entity* currEntPtr)
{
    intptr_t modBaseAddr{ (intptr_t)GetModuleHandleW(L"client.dll") };

    Entity* localPlayer{ *(Entity**)(modBaseAddr + 0x16D5C80) };

    const intptr_t isEntityCode{ *(intptr_t*)localPlayer };

    if (currEntPtr == nullptr && (intptr_t)currEntPtr == NULL)
        return false;

    // check if this is a player
    const intptr_t currIsEntCode{ *(intptr_t*)currEntPtr };

    if (currIsEntCode != isEntityCode)
        return false;

    if (currEntPtr->health < 0)
        return false;

    if (localPlayer->team_variable == currEntPtr->team_variable)
        return false;

    return true;
}

std::vector<Entity*> GetTargetList(Entity* localPlayer, EntityList* entityList)
{
    std::vector<Entity*> target{};

    //iterate entList
    for (intptr_t i{ 0 }; i < 32; ++i)
    {
        //filter a good target
        Entity* currEntPtr{ entityList->entity[i] };

        if (!IsGoodTarget(currEntPtr))
            continue;

        target.push_back(currEntPtr);
    }

    return target;
}

Entity* GetNearestTarget(std::vector<Entity*>)
{

    return (Entity*)0x0;
}

Vector3 GetTargetAngle(Entity* target)
{

    return Vector3();
}

DWORD WINAPI MainThread(HMODULE hModule)
{
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);

    intptr_t modBaseAddr{ (intptr_t)GetModuleHandleW(L"client.dll") };



    Entity* localPlayer{ *(Entity**)(modBaseAddr + 0x16D5C80) };
    
    EntityList* entityList{ (EntityList*)(modBaseAddr + 0x16D5C80) };

    while (!GetAsyncKeyState(VK_DELETE) & 1)
    {
        std::vector<Entity*> target{ GetTargetList(localPlayer, entityList) };

        //TODO
        Entity* nearestTarget{ GetNearestTarget(target) };

        //TODO
        // calculate target angle
        GetTargetAngle(nearestTarget);
        
        //TODO
        // change my angles to aim at target
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

