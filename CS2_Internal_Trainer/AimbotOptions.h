#pragma once
#include "Pawn.h"
#include <Windows.h>

namespace AimbotOptions
{
    inline Pawn* targetLocked{ nullptr };

    inline bool bTargetLock{ false };
    inline bool bAimbot{ true };

    inline int smoothValue{ 0 };
    inline int fovValue{ 50 };

    bool IsOptionChanged();
};