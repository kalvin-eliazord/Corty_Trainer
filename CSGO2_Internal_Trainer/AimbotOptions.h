#pragma once
#include "Entity.h"

namespace AimbotOptions
{
    static inline Entity* targetLocked{ nullptr };
    static inline bool bTargetLock{ false };
    static inline int smoothValue{ 0 };
    static inline int fovValue{ 50 };
};