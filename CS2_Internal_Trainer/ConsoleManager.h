#pragma once
#include "CheatOptions.h"
#include <iostream>
#include <vector>

namespace ConsoleManager
{
    inline FILE* file{nullptr};

    // Console info updates
    inline bool bLobbyStart{ true };
    inline bool bInGameStart{ true };
    inline bool bConsoleChanged{ true };

    void InitConsole();
    void PrintMyName();
    void DestroyConsole();
    void PrintCheatOptions();
    void PrintErrorPtrInit(std::map <std::string, intptr_t > pAddresses);
};