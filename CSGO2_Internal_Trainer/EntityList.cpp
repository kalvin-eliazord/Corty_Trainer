#include "EntityList.h"

std::vector<Entity*> EntityList::GetTargetList()
{
    std::vector<Entity*> targetList{};

    // *2 because there is one object generated for each entity in the list
    for (int i{ 0 }; i < (this->nbEntAlive * 2); ++i)
    {
        Entity* currEntity{ this->entity[i] };

        if (!this->IsGoodTarget(currEntity))
            continue;

        targetList.push_back(currEntity);
    }

    return targetList;
}

void EntityList::SetNbEntAlive(int pNbEntAlive)
{
    this->nbEntAlive = pNbEntAlive;
}

void EntityList::SetNbEntAlive()
{
    const int ct_EntAlive = static_cast<int>(MemoryManager::GetDynamicAddr(GameOffset::Client::nbEntitiesBaseAddrPtr,
        {
            0x598,
            0x3A0,
            0x1D8,
            0x8,
            0xB8,
            0x20CC
        }));

    const int t_EntAlive = static_cast<int>(MemoryManager::GetDynamicAddr(GameOffset::Client::nbEntitiesBaseAddrPtr,
        {
            0x598,
            0x3A0,
            0x1D8,
            0x8,
            0xB8,
            0x20C8
        }));

    this->nbEntAlive = ct_EntAlive + t_EntAlive;
}

int EntityList::GetNbEntAlive()
{
    return this->nbEntAlive;
}

bool EntityList::IsGoodTarget(Entity* entityPtr)
{
    Entity* localPlayer{ LocalPlayer::Get() };

    const intptr_t lpEntityId{ *(intptr_t*)localPlayer };

    // Not an Entity
    if (*(intptr_t*)entityPtr != lpEntityId)
        return false;

    // iteration empty
    if (entityPtr == nullptr && (intptr_t)entityPtr == NULL)
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