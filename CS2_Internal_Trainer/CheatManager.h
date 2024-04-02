#pragma once
#include <Windows.h>
#include "EntityList.h"
#include "TargetManager.h"
#include "LocalPlayer.h"
#include "GamePointer.h"
#include "ConsoleManager.h"
#include "AimbotOptions.h"

namespace CheatManager
{
	inline bool bAimbot{ false };
	//inline bool bNoRecoil{ false };
	//inline bool bESP {false};

	bool StartAimbot();
	//bool StartNoRecoil();
	//bool StartEsp();
};