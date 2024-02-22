#pragma once
#include "header.h"

namespace GameOffset
{
	namespace Client
	{
		static const intptr_t modBaseAddr         { (intptr_t)GetModuleHandleW(L"client.dll") };
		static const intptr_t localPlayer      { modBaseAddr + 0x1729348 };
		static const intptr_t* entitiesListPtr{ *(intptr_t**)(modBaseAddr + 0x1736BA8) };
		static intptr_t nbEntitiesBaseAddrPtr{ *(intptr_t*)(modBaseAddr + 0x1698800) };

		static float* lp_Pitch_Input{ (float*)(modBaseAddr + 0x19233E0) };
		static float* lp_Yaw_Input  { (float*)(modBaseAddr + 0x19233E4) };
		static int*   gameStateIdPtr{ (int*)(modBaseAddr + 0x17292B0) };
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