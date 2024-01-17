#include "EntityManager.h"

Entity* EntityManager::GetLocalPlayer()
{
	return *(Entity**)(Offset::clientModBaseAddr + Offset::localPlayer);
}
