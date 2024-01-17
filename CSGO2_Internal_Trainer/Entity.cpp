#include "Entity.h"

Entity* Entity::GetLocalPlayer()
{
    return *(Entity**)(Offset::Client::localPlayer);
}