#pragma once
#include "header.h"

namespace GameOffset
{
	namespace Client
	{
		static const intptr_t modBaseAddr         { (intptr_t)GetModuleHandleW(L"client.dll") };
		static intptr_t cPredictionBaseAddr{ *(intptr_t*)(modBaseAddr + 0x19FDDD8) };

		static const intptr_t localPlayer      { modBaseAddr + 0x1734B60 };
		static const intptr_t* entitiesListPtr { *(intptr_t**)(modBaseAddr + 0x1734B48) };

		static int*   gameStateIdPtr{ (int*)(cPredictionBaseAddr + 0xA0) };
		static float* lp_Pitch_Input{ (float*)(modBaseAddr + 0x191F100) };
		static float* lp_Yaw_Input  { (float*)(modBaseAddr + 0x191F104) };
	}
	
	namespace Engine2
	{
		static const intptr_t modBaseAddr{ (intptr_t)GetModuleHandleW(L"engine2.dll") };
	}

	namespace Panorama
	{
		static const intptr_t modBaseAddr{ (intptr_t)GetModuleHandleW(L"panorama.dll") };
	}
	
	namespace Server
	{
		static const intptr_t modBaseAddr{ (intptr_t)GetModuleHandleW(L"server.dll") };
		static int* nbEntitiesPtr{ (int*)(modBaseAddr + 0x1305B74) };
	}
};