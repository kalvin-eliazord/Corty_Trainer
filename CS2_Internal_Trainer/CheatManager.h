#pragma once
#include <Windows.h>
#include "EntityList.h"
#include "TargetManager.h"
#include "LocalPlayer.h"
#include "BasicMath.h"
#include "GamePointer.h"
#include "ConsoleManager.h"
#include "AimbotOptions.h"

namespace CheatManager
{
	inline bool bAimbot{ true };
	//bool bESP {true};

	bool StartAimbot();
	//void StartEsp();
};