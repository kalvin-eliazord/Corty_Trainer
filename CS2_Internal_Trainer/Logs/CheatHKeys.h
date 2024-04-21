#pragma once
#include <Windows.h>
#include "Entity.h"

namespace CheatHKeys
{
    inline Controller* cBaseLocked{ nullptr };

    inline bool bTargetLock{ false };
    inline bool bHeadPos{ true };
    inline bool bAimbot{ true };
    inline bool bESP{ true };

    //inline bool bRCS{ true };

    inline int smoothValue{ 0 };
    inline int fovValue{ 50 };

    bool IsOptionChanged();
};