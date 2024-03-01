#pragma once
#include "header.h"

namespace GameOffset
{
	namespace Client
	{
		static const intptr_t modBaseAddr         { (intptr_t)GetModuleHandleW(L"client.dll") };
		static const intptr_t localPlayer      { modBaseAddr + 0x173DBC0 };
		static const intptr_t entitiesListBaseAddr{ (modBaseAddr + 0x173DBC0) };
		static const intptr_t csgoHudBaseAddr{ *(intptr_t*)(modBaseAddr + 0x1976ED0) };

		static float* lp_Pitch_Input{ (float*)(modBaseAddr + 0x1929730) };
		static float* lp_Yaw_Input  { (float*)(modBaseAddr + 0x1929734) };
		static int*   gameStateIdPtr{ (int*)(modBaseAddr + 0x1730080) };
		static int*   gameTypeIdPtr{ (int*)(0x7FF993202509) };
	}
	
	namespace Engine2
	{
	//	static const intptr_t modBaseAddr{ (intptr_t)GetModuleHandleW(L"engine2.dll") };
	}

	namespace Panorama
	{
//		static const intptr_t modBaseAddr{ (intptr_t)GetModuleHandleW(L"panorama.dll") };
	}
};