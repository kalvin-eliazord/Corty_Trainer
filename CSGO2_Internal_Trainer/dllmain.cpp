#include "header.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);

    // init addr
    Entity* localPlayer{ LocalPlayer::Get()};
    EntityList* entitiesList{ (EntityList*)Offset::Client::entitiesList };

    // To prevent read access violation errors.
    intptr_t* gameStateIdPtr{Offset::Client::gameStateID};
    constexpr int inGameStateID{ 8 };

    EntityNameList* entNameList{(EntityNameList*)0x02794ADF0880 };

    int nbEntities{ NULL };

    std::cout << "NOT IN GAME \t \r";

    do 
    {
        nbEntities = entitiesList->GetNbEntAlive();
        Sleep(5);
    } while (!nbEntities);

    while (!GetAsyncKeyState(VK_DELETE) & 1)
    {
        if (*gameStateIdPtr == inGameStateID)
        {
            system("CLS");
            std::cout << "IN GAME \t \r";

            std::vector<Entity*> targetList{};

            // filter good target
            for (int i{ 0 }; i < (nbEntities * 2); ++i)
            {
                Entity* currEntity{ entitiesList->entity[i] };

                 // check if the entity is valid
                 if (entitiesList->IsGoodTarget(currEntity))
                     targetList.push_back(currEntity);
                 else
                    continue;
            }

            Vector3 targetAngles{};
            int nearestTargetIndex{-1};

            Aimbot aimbot{};

            // Filter the nearest entity
            for (int i{ 0 }; i < targetList.size(); ++i)
            {
                aimbot.SetNearestTarget(targetList[i], i);
                nearestTargetIndex = aimbot.GetNearestTargetIndex();
            }

            // calculate target angle
            if (nearestTargetIndex != -1)
            {
                targetAngles = aimbot.GetTargetAngle(targetList[nearestTargetIndex]);

                //BUG = ANGLES DOESNT CHANGE IG
                float* lpPitch{(float*) (Offset::Client::modBaseAddr + 0x1880DC0) };
                float* lpYaw{ (float*)(Offset::Client::modBaseAddr + 0x1880DC4) };

                 *lpPitch = targetAngles.x;
                 *lpYaw = targetAngles.y;

                 //   localPlayer->angles.x = targetAngles.x;
                   // localPlayer->angles.y = targetAngles.y;
            }

        }
        else
        {
            system("CLS");
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

