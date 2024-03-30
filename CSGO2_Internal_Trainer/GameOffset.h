#pragma once
#include "EntityList.h"


namespace GameOffset
{
	char* GetPatternMatch(char* pPattern, char* pMask, char* pSrc, intptr_t pRegionSize);
	char* ScanModuleRegion(char* pPattern, char* pMask, char* pSrc, size_t pSrcSize);
	char* GetGamePointer(char* pPattern, wchar_t* pModName);

	bool UpdatePointers();

	namespace Client
	{
		intptr_t* csgoHudBaseAddr{ nullptr };
		intptr_t* csGoInputBaseAddr{ nullptr };
		intptr_t* CWatchableBaseAddr{ nullptr };

		static EntityList* entityListPtr{ nullptr };

		static Entity* localPlayerPtr{ nullptr };
		static float* lp_Pitch_Input{ nullptr };
		static float* lp_Yaw_Input  {nullptr };

		static int* gameStateIdPtr{ nullptr };
		static int8_t* gameTypeIdPtr{ nullptr };
	}
	
	namespace Offset
	{
		constexpr inline intptr_t gameTypeId{ 0xB60 };
		constexpr inline intptr_t gameStateId{ 0x78 };
		constexpr inline intptr_t lpBaseAddr{ 0x8 };
		constexpr inline intptr_t lp_Pitch{ 0x5390 };
		constexpr inline intptr_t lp_Yaw{ 0x5394 };
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