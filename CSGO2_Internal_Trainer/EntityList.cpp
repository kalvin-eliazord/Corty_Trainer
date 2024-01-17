#include "EntityList.h"

std::vector<Entity*> EntityList::GetTargetList(Entity* localPlayer, EntityList* entityList)
{
    std::vector<Entity*> target{};

    /*
    //iterate entList
    for (intptr_t i{ 0 }; i < ; ++i)
    {
        //filter a good target
        Entity* currEntPtr{ entityList->entity[i] };

        if (!IsGoodTarget(currEntPtr))
            continue;

        target.push_back(currEntPtr);
    }
    */
    return target;
}

bool EntityList::IsGoodTarget(Entity* currEntPtr)
{
    Entity* localPlayer{ EntityManager::GetLocalPlayer() };

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

    return ct_AliveNb + t_AliveNb;
}

void EntityList::PushValidEntity()
{
	Entity* localPlayer{EntityManager::GetLocalPlayer()};
}
