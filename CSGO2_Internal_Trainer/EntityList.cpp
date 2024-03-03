#include "EntityList.h"

std::vector<Entity*> EntityList::GetTargetList(Entity* pLocalPlayer, int pNbEntAlive)
{
    std::vector<Entity*> targetList{};

    const int newNbEntAlive{ GetNbEntAlive() };

    // Update number of entity to iterate
    if(newNbEntAlive > pNbEntAlive)
        pNbEntAlive = newNbEntAlive;

    // *2 because there is one object generated for each entity in the list
    for (int i{ 0 }; i < (pNbEntAlive * 2); ++i)
    {
        Entity* currEntity{ (this->entity[i]) };

        if (!this->IsGoodTarget(pLocalPlayer, currEntity))
            continue;

        targetList.push_back(currEntity);
    }

    return targetList;
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
    if (!entityPtr or !*(intptr_t*)entityPtr or !(intptr_t)entityPtr)
        return false;

    //const intptr_t playerEntityId{ (intptr_t)pLocalPlayer };
    //const intptr_t currEntId{ (intptr_t)entityPtr };

    // Not an Entity
   //if (currEntId != playerEntityId)
     //   return false;

    // Entity dead
    if (entityPtr->health < 1)
        return false;

    // Same team as LP
    if (pLocalPlayer->team_variable == entityPtr->team_variable)
        return false;

    // If Entity is behind the wall

    return true;
}