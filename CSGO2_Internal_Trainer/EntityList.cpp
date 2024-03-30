#include "EntityList.h"

std::vector<Entity*> EntityList::GetTargetList(Entity* pLocalPlayer, int8_t* pGameType)
{
    std::vector<Entity*> targetList{};

    for (int i{ 0 }; i < 32; ++i)
    {
        Entity* currEntity{ (this->entity[i]) };

        if (!this->IsGoodTarget(pLocalPlayer, currEntity, pGameType))
            continue;

        targetList.push_back(currEntity);
    }

    return targetList;
}

bool EntityList::IsGoodTarget(Entity* pLocalPlayer, Entity* entityPtr, int8_t* pGameType)
{
    // iteration empty
    if (!entityPtr or !*(intptr_t*)entityPtr or !(intptr_t)entityPtr)
        return false;

    // Entity dead
    if (entityPtr->health < 1)
        return false;

    constexpr int8_t deathmatchType{ 39 };

    // don't check for team if there is no team
    if (*pGameType != deathmatchType)
    {
        // Same team as LP
        if (pLocalPlayer->team_variable == entityPtr->team_variable)
            return false;
    }

    // If Entity is behind the wall

    return true;
}