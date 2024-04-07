#pragma once
#include "Pawn.h"
#include "ConsoleManager.h"

namespace AimbotOptions
{
    inline Pawn* targetLocked{ nullptr };
    inline bool bTargetLock{ false };
    inline int smoothValue{ 0 };
    inline int fovValue{ 50 };

    void OptionsCheck();
};