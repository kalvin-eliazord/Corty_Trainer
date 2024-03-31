#pragma once
#include <iostream>
#include <Windows.h>
#include "GameChecker.h"
#include "AimbotOptions.h"

struct ConsoleManager
{
    FILE* file{};

    // Console info updates
    bool bConsoleChanged{ false };
    bool bWaitingLobbyMsg{ true };
    bool bStartingInGame{ true };

    ConsoleManager();
    ~ConsoleManager();

    void PrintCheatOptions();
};