#pragma once
#include <Windows.h>
#include "GameState.h"

namespace CheatHKeys
{
    inline bool bTargetLock{ false };
    inline bool bHeadPos{ true };
    inline bool bAimbot{ false };
    inline bool bESP{ true };
    inline bool bTeamCheck{ true };
    //inline bool bRCS{ true };

    inline int smoothValue{ 0 };
    inline int fovValue{ 50 };

    inline bool bInitTeamCheck{ false };
    bool IsOptionChanged();
    bool SetTeamCheckDefaultValue(bool pIsDeathMatch);
};