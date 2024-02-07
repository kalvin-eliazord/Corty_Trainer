#include "EntityList.h"

int EntityList::GetNbEntAlive()
{
    int t_AliveNb{ (int)MemoryManager::GetDynamicAddr(Offset::Panorama::modBaseAddr,
{
    0x499AA8,
    0x608,
    0x2B0,
    0x8,
    0xB8,
    0x190,
    0x148,
    0x18,
    0x30,
    0x0,
    0x78,
    0x258,
    0x8,
    0x8,
    0x110,
    0x120,
    0x8,
    0x118,
    0x8,
    0x20CC
}) };

    int ct_AliveNb{ (int)MemoryManager::GetDynamicAddr(Offset::Panorama::modBaseAddr,
    {
    0x499AA8,
    0x608,
    0x2B0,
    0x8,
    0xB8,
    0x190,
    0x148,
    0x18,
    0x30,
    0x0,
    0x78,
    0x258,
    0x8,
    0x8,
    0x110,
    0x120,
    0x8,
    0x118,
    0x8,
    0x20C8
    }) };

    return (ct_AliveNb + t_AliveNb);
}

bool EntityList::IsGoodTarget(Entity* entityPtr)
{
    Entity* localPlayer{ LocalPlayer::Get() };

    const intptr_t lpEntityId{ *(intptr_t*)localPlayer };

    if (entityPtr == nullptr && (intptr_t)entityPtr == NULL)
        return false;

    // Not an Entity
    if (*(intptr_t*)entityPtr != lpEntityId)
        return false;

    // Entity dead
    if (entityPtr->health == 0)
        return false;

    // Same team as LP
    if (localPlayer->team_variable == entityPtr->team_variable)
        return false;

    // If Entity is behind the wall

    return true;
}