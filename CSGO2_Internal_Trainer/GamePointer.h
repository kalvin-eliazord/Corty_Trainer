#pragma once
#include <windows.h>
#include <Psapi.h>
#include <cstdint>

namespace GamePointer
{
	char* GetPatternMatch(char* pPattern, char* pMask, char* pSrc, intptr_t pRegionSize);
	char* ScanModuleRegion(char* pPattern, char* pMask, char* pSrc, size_t pSrcSize);
	intptr_t* GetGamePointer(char* pPattern, wchar_t* pModName);
	bool InitializePointers();
	bool InitializePointersInGame();

	inline intptr_t* entityListPtr{ nullptr };
	inline intptr_t* localPlayerPtr{ nullptr };
	inline float* lp_Pitch_Input{ nullptr };
	inline float* lp_Yaw_Input  {nullptr };

	inline int* gameStateIdPtr{ nullptr };
	inline int8_t* gameTypeIdPtr{ nullptr };
	
	namespace Offset
	{
		constexpr inline intptr_t gameTypeId{ 0xB60 };
		constexpr inline intptr_t gameStateId{ 0x128 };
		constexpr inline intptr_t lpBaseAddr{ 0x8 };
		constexpr inline intptr_t lp_Pitch{ 0x418 };
		constexpr inline intptr_t lp_Yaw{ 0x414 };
	}
};