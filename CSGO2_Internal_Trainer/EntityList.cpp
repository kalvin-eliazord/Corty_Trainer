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

int EntityList::GetNbOfEntAlive()
{
	return 0;
}

void EntityList::PushValidEntity()
{
	Entity* localPlayer{EntityManager::GetLocalPlayer()};
}
