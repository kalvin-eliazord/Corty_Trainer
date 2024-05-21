#pragma once
#include "CheatHKeys.h"
#include <iostream>
#include <map>
#include <vector>

namespace ConsoleCheatMenu
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
    std::string_view GetTargetedPart();
    void PrintErrorPtrInit(const std::map <std::string_view, intptr_t >& pAddresses);
};