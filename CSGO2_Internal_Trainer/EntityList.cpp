#include "EntityList.h"

std::vector<Entity*> EntityList::GetTargetList(Entity* pLocalPlayer)
{
    std::vector<Entity*> targetList{};

    const int newNbEntAlive{ GetNbEntAlive() };

    // Update number of entity to iterate
    if(newNbEntAlive > this->nbEntAlive)
        SetNbEntAlive(newNbEntAlive);

    // *2 because there is one object generated for each entity in the list
    for (int i{ 0 }; i < (this->nbEntAlive * 2); ++i)
    {
        Entity* currEntity{ this->entity[i] };

        if (!this->IsGoodTarget(pLocalPlayer, currEntity))
            continue;

        targetList.push_back(currEntity);
    }

    return targetList;
}

void EntityList::SetNbEntAlive(int pNbEntAlive)
{
    this->nbEntAlive = pNbEntAlive;
}

int EntityList::GetNbEntAlive()
{
    const int ct_EntAlive = static_cast<int>(MemoryManager::GetDynamicAddr(GameOffset::Client::csgoHudBaseAddr,
        { 
            0xB8,
            0x20CC 
        }));

    const int t_EntAlive = static_cast<int>(MemoryManager::GetDynamicAddr(GameOffset::Client::csgoHudBaseAddr,
        {
            0xB8,
            0x20C8
        }));

    return ct_EntAlive + t_EntAlive;
}

bool EntityList::IsGoodTarget(Entity* pLocalPlayer, Entity* entityPtr)
{
    // iteration empty
    if (entityPtr == nullptr && (intptr_t)entityPtr == NULL)
        return false;

    //const intptr_t playerEntityId{ *(intptr_t*)pLocalPlayer };
    //const intptr_t currIndexId{ *(intptr_t*)entityPtr };

    // Not an Entity
    //if (currIndexId != playerEntityId)
      //  return false;

    // Entity dead
    if (entityPtr->health < 1 or entityPtr->health > 100)
        return false;

    // Same team as LP
    if (pLocalPlayer->team_variable == entityPtr->team_variable)
        return false;

    // If Entity is behind the wall

    return true;
}