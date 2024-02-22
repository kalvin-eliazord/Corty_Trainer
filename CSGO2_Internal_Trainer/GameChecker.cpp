#include "GameChecker.h"

void GameChecker::CheckGameState(EntityList* pEntityList)
{
    if (*GameChecker::gameStateIdPtr == GameChecker::inGameId)
    {
        Sleep(5000);
        pEntityList->SetNbEntAlive();
    }
}