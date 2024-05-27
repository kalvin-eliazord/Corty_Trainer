#pragma once
#include "GameState.h"
#include <Windows.h>
#include <iostream>
#include <map>
#include <vector>

namespace ConsoleMenu
{
    inline FILE* file{nullptr};

    inline bool bTargetLock{ false };
    inline bool bHeadPos{ true };
    inline bool bAimbot{ false };
    inline bool bESP{ true };
    inline bool bTeamCheck{ true };
    //inline bool bRCS{ true };

    inline int smoothValue{ 0 };
    inline int fovValue{ 25 };

    inline bool bInitTeamCheck{ false };
    bool IsOptionChanged();
    bool SetDefaultTeamCheck(bool pIsDeathMatch);

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