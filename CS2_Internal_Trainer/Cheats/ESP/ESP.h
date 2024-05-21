#pragma once
#include "MyD3D_Utils.h"
#include "MyD3D11.h"
#include "LocalPlayer.h"
#include "Entity.h"
#include "CheatHKeys.h"
#include "GamePointers.h"
#include <vector>

extern MyD3D11 g_myD3d11;

namespace ESP
{
	std::vector<Entity> GetValidTargets();
	bool IsGoodTarget(Entity* pCurrEntPtr);
	bool SnapLineTo(Vector3 pEntPos, float pWinWidth, float pWinHeight);
	bool Start();
};