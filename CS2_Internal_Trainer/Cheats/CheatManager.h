#pragma once
#include "Aimbot.h"
#include "ESP.h"
#include "ConsoleMenu.h"
#include "MyPointers.h"
#include "GameState.h"
#include "TrampHook.h"
#include "MyD3D11.h"

namespace CheatManager
{
	inline const bool* bHookPtr{};
	bool InitHook();
	bool Run();
};