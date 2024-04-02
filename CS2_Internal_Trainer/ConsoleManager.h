#pragma once
#include <iostream>
#include <Windows.h>
#include "GameChecker.h"
#include "CheatManager.h"
#include "AimbotOptions.h"

namespace ConsoleManager
{
    inline FILE* file{nullptr};

    // Console info updates
    inline bool bConsoleChanged{ false };
    inline bool bInGameStart{ true };
    inline bool bLobbyStart{ true };

    void InitConsole();
    void DestroyConsole();
    void PrintCheatOptions();
};