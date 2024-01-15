#include "header.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);

    uintptr_t modBaseAddr{ (uintptr_t)GetModuleHandleW(L"client.dll") };

    Entity* localPlayer{ *(Entity**)(modBaseAddr + 0x16D5C80) };
    
    int* entityNb{nullptr};

    //iterate entList
    for (int i{ 0 }; i < *entityNb; ++i)
    {

        //filter a good target
    }

    // calculate target angle

    // change my angles to aim at target


    while (!GetAsyncKeyState(VK_DELETE) & 1)
    {
       // std::cout << localPlayer->health << "\r";
        localPlayer->angles.x = 50.0f;
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

