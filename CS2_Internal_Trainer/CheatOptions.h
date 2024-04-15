#pragma once
#include <Windows.h>
#include "Entity.h"

namespace CheatOptions
{
    inline Entity* entLocked{ nullptr };

    inline bool bTargetLock{ false };
    inline bool bAimbot{ true };
    inline bool bESP{ true };
    //inline bool bRCS{ true };

    inline int smoothValue{ 0 };
    inline int fovValue{ 50 };

    bool IsOptionChanged();
};