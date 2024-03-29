#pragma once
#include "header.h"

namespace GameOffset
{
	int   GetPatternLen(char* pPattern);
	char* GetPatternMatch(char* pPattern, char* pMask, char* pSrc, intptr_t pRegionSize);
	char* ScanModuleRegion(char* pPattern, char* pMask, char* pSrc, size_t pSrcSize);
	char* GetGamePointers(char* pPattern, char* pMask, wchar_t* pModName);

	namespace Client
	{
		static const HMODULE modBaseAddr       { GetModuleHandleW(L"client.dll") };
		//todo
		//static const intptr_t csgoHudBaseAddr{ *(intptr_t*)(modBaseAddr + 0x197AFF0) };

		//static float* lp_Pitch_Input{ (float*)(modBaseAddr + 0x192D860) };
		//static float* lp_Yaw_Input  { (float*)(modBaseAddr + 0x192D864) };
		//static int* gameStateIdPtr{ (int*)(modBaseAddr + 0x1734120) };
		//static int8_t* gameTypeIdPtr{ (int8_t*)(modBaseAddr + 0x16F0C22) };
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