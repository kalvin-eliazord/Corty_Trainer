#include "EntityList.h"

int EntityList::GetNbEntAlive()
{
    int t_AliveNb{ (int)MemoryManager::GetDynamicAddr(Offset::Client::cNetworkSerialiserPtr,
{
    0x20,
    0x18,
    0xA8,
    0x8,
    0x30,
    0x0,
    0x8,
    0x8,
    0x8,
    0x38,
    0x190,
    0x68,
    0x8,
    0x8,
    0x218,
    0x18,
    0x8,
    0x50,
    0x8,
    0x20CC
}) };

    int ct_AliveNb{ (int)MemoryManager::GetDynamicAddr(Offset::Client::cNetworkSerialiserPtr,
    {
        0x20,
        0x18,
        0xA8,
        0x8,
        0x30,
        0x0,
        0x8,
        0x8,
        0x8,
        0x38,
        0x190,
        0x68,
        0x8,
        0x8,
        0x218,
        0x18,
        0x8,
        0x50,
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
    if (entityPtr->health < 0)
        return false;

    // Same team as LP
    if (localPlayer->team_variable == entityPtr->team_variable)
        return false;

    // If Entity is behind the wall

    return true;
}