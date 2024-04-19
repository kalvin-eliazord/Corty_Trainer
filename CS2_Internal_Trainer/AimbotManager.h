#pragma once
#include "LocalPlayer.h"
#include "TargetManager.h"
#include "CheatHKeys.h"

namespace AimbotManager
{
	bool Start();
	bool TargetLockShot();
	bool TargetShot(Entity pTarget);
};