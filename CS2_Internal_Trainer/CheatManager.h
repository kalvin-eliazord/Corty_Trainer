#pragma once
#include <Windows.h>
#include "TargetManager.h"
#include "LocalPlayer.h"
#include "GamePointer.h"
#include "ConsoleManager.h"
#include "AimbotOptions.h"

namespace CheatManager
{
	inline bool bAimbot{ false };
	//inline bool bESP {false};

	bool StartAimbot();
	//bool StartEsp();
};