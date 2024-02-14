#include "EntityList.h"

std::vector<Entity*> EntityList::GetTargetList()
{
    std::vector<Entity*> targetList{};

    for (int i{ 0 }; i < (this->nbEntities * 2); ++i)
    {
        Entity* currEntity{ this->entity[i] };

        if (!this->IsGoodTarget(currEntity))
            continue;

        targetList.push_back(currEntity);
    }

    return targetList;
}

int EntityList::GetNbEntAlive()
{
    return this->nbEntities;
}

void EntityList::SetNbEntAlive()
{
    this->nbEntities = *GameOffset::Server::nbEntitiesPtr;
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