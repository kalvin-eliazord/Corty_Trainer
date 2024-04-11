#pragma once
#include <windows.h>
#include <Psapi.h>
#include <cstdint>

namespace GamePointer
{
	intptr_t* GetPatternPointer(intptr_t* pPatternMatch);
	int GetNumberHex (const char* pPattern);
	intptr_t* ScanRegion(const char* pPattern, char* pSrc, intptr_t pRegionSize);
	intptr_t* SearchGoodModRegion(const char* pPattern, char* pSrc, size_t pSrcSize);
	intptr_t* GetPatternMatch(const char* pPattern, const HMODULE pModName);

	bool GetGameTypeIdPtr(HMODULE hModule);
	bool GetGameStateIdPtr(HMODULE hModule);
	bool GetViewAnglesPtr(HMODULE hModule);
	bool GetLocalPlayerPawnPtr(HMODULE hModule);
	bool GetLocalPlayerContPtr(HMODULE hModule);
	bool GetEntListBaseAddrPtr(HMODULE hModule);

	SIZE_T GetModuleSize(HMODULE pModule);
	bool InitializePointers();
	bool InitGameTypeIdPtr();

	
	inline intptr_t* localPlayerPawnPtr{ nullptr };
	inline intptr_t* entityListBasePtr{ nullptr };
	inline intptr_t* localPlayerContPtr{ nullptr };
	inline float* lp_Pitch_Input{ nullptr };
	inline float* lp_Yaw_Input  {nullptr };

	inline int_least8_t* gameStateIdPtr{ nullptr };
	inline int_least8_t* gameTypeIdPtr{ nullptr };

	namespace Offset
	{
		constexpr inline intptr_t entityList{ 0x10 };
		constexpr inline intptr_t gameStateId{ 0xA0 };
		constexpr inline intptr_t gameTypeId{ 0x32 };
		constexpr inline intptr_t lp_Pitch{ 0x5390 };
		constexpr inline intptr_t lp_Yaw{ 0x5394 };
		constexpr inline intptr_t lp_Pawn{ 0x138 };
	}

	namespace Signature
	{
		static constexpr char InputSystem[]{ "\x48\x8B\?\?\?\?\?\x48\x8D\?\?\?\x45\x33\?\xE8\?\?\?\?\xF2\x0F" };
		static constexpr char CPrediction[]{ "\x48\x8D\?\?\?\?\?\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\?\x8B\x0D" };
		static constexpr char EntityList[]{ "\x48\x8B\x0D\?\?\?\?\x48\x89\x7C\x24\?\x8B\xFA\xC1\xEB" };
		static constexpr char LocalPlayerController[]{ "\x48\x8B\x05\?\?\?\?\x48\x85\xC0\x74\?\x8B\x88" };
		static constexpr char ViewAngles[]{ "\x48\x8B\x0D\?\?\?\?\x48\x8B\x01\x48\xFF\x60\x30" };
		static constexpr char WeaponList[]{"\x48\x89\x0D\?\?\?\?\x48\x83\xC4\?\x48\xFF\?\?\?\?\?\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x8B\?\?\?\?\?\x48\x8D\?\?\?\?\?\xE9\?\?\?\?\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x8D\?\?\?\?\?\xE9\?\?\?\?\xCC\xCC\xCC\xCC\x48\x83\xEC\?\x65\x48\?\?\?\?\?\?\?\x8B\x0D\?\?\?\?\xBA\?\?\?\?\x48\x8B\?\?\x8B\x04\?\x39\x05\?\?\?\?\x7F\?\x48\x8D\?\?\?\?\?\x48\x89\?\?\?\?\?\x48\x8D\?\?\?\?\?\x48\x89\?\?\?\?\?\x48\x83\xC4\?\xC3\x48\x8D\?\?\?\?\?\xE8\?\?\?\?\x83\x3D\x5D\x20\x85\x01"};
	}
};