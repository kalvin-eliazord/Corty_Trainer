#include "EntityManager.h"

Entity* EntityManager::GetLocalPlayer()
{
	return *(Entity**)(Offset::Client::localPlayer);
}