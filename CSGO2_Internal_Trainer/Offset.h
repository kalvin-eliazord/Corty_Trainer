#pragma once
#include "header.h"

namespace Offset
{
	namespace Client
	{
		static const intptr_t modBaseAddr  { (intptr_t)GetModuleHandleW(L"client.dll") };
		static const intptr_t localPlayer  { modBaseAddr + 0x16E1C70 };
		static const intptr_t entitiesList { modBaseAddr + 0x16E1C68 };
		static intptr_t cNetworkSerialiserPtr{ *(intptr_t*)(modBaseAddr + 0x181C808) };
		static const float* lp_Input_Pitch{ (float*)(modBaseAddr + 0x1890F30) };
		static const float* lp_Input_Yaw  { (float*)(modBaseAddr + 0x1890F34) };

	}

	namespace Engine2
	{
		static const intptr_t modBaseAddr{ (intptr_t)GetModuleHandleW(L"engine2.dll") };
		static intptr_t* cPredictionBaseAddr{ *(intptr_t**)(modBaseAddr + 0x542C48) };
		static intptr_t* gameStateID{ (intptr_t*)((intptr_t)cPredictionBaseAddr + 0xA0) };
	}

	namespace Panorama
	{
		static const intptr_t modBaseAddr{ (intptr_t)GetModuleHandleW(L"panorama.dll") };
	}
	
};