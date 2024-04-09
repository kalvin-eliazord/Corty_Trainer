#pragma once
#include "AimbotOptions.h"
#include <iostream>

namespace ConsoleManager
{
    inline FILE* file{nullptr};

    // Console info updates
    inline bool bConsoleChanged{ true };
    inline bool bInGameStart{ true };
    inline bool bLobbyStart{ true };

    void InitConsole();
    void DestroyConsole();
    void PrintMyName();
    void PrintErrorPtrInit();
    void PrintCheatOptions();
};