#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <cstdint>
#include <map>
#include <string>

namespace GamePointer
{
	inline intptr_t* SteamOverlayPtr{};

	inline intptr_t* CGameEntityPtr{};
	inline intptr_t* entityListBasePtr{};

	inline float* lp_Yaw_Input{};
	inline float* lp_Pitch_Input{};
	inline intptr_t* localPlayerContPtr{};

	inline int16_t* gameTypeIdPtr{};
	inline int16_t* gameStateIdPtr{};

	inline std::map<std::string, intptr_t> pointersValue;

	bool CheckPointers();
	bool InitGameTypeIdPtr();
	bool InitializePointers();
	SIZE_T GetModuleSize(HMODULE pModule);

	void GetSteamOverlayPtr(HMODULE hModule);

	void GetCGameEntityPtr(HMODULE hModule);
	void GetEntListBaseAddrPtr(HMODULE hModule);

	void GetGameTypeIdPtr(HMODULE hModule);
	void GetGameStateIdPtr(HMODULE hModule);

	void GetViewAnglesPtr(HMODULE hModule);
	void GetLocalPlayerContPtr(HMODULE hModule);

	intptr_t* GetPatternMatch(const char* pPattern, const HMODULE pModName);
	intptr_t* SearchGoodModRegion(const char* pPattern, char* pSrc, size_t pSrcSize);
	intptr_t* ScanRegion(const char* pPattern, char* pSrc, intptr_t pRegionSize);
	intptr_t* GetPatternPointer(intptr_t* pPatternMatch);
	int GetNumberHex(const char* pPattern);

	namespace Offset
	{
		constexpr inline DWORD Lp_Yaw{ 0x5394 };
		constexpr inline DWORD Lp_Pawn{ 0x138 };
		constexpr inline DWORD EntityList{ 0x10 };
		constexpr inline DWORD GameTypeId{ 0x32 };
		constexpr inline DWORD Lp_Pitch{ 0x5390 };
		constexpr inline DWORD BoneArray{ 0x1E0 };
		constexpr inline DWORD GameStateId{ 0xA0 };
		constexpr inline DWORD bSpottedId{ 0x16A4 };
		constexpr inline DWORD GameSceneNode{ 0x318 };
	}

	namespace Signature
	{
		constexpr char ViewAngles[]{ "\x48\x8B\x0D\?\?\?\?\x48\x8B\x01\x48\xFF\x60\x30" };
		constexpr char CGameEntity[]{ "\x48\x8B\x0D\?\?\?\?\x48\x89\x7C\x24\?\x8B\xFA\xC1\xEB" };
		constexpr char LocalPlayerController[]{ "\x48\x8B\x05\?\?\?\?\x48\x85\xC0\x74\?\x8B\x88" };
		constexpr char InputSystem[]{ "\x48\x8B\?\?\?\?\?\x48\x8D\?\?\?\x45\x33\?\xE8\?\?\?\?\xF2\x0F" };
		constexpr char CPrediction[]{ "\x48\x8D\?\?\?\?\?\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\?\x8B\x0D" };
		constexpr char SteamOverlay[]{ "\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x41\x56\x48\x83\xEC\x20\x41\x8B\xE8\x8B\xF2\x4C\x8B\xF1\x41\xF6\xC0\x01" };
		constexpr char WeaponList[]{ "\x48\x89\x0D\?\?\?\?\x48\x83\xC4\x28\x48\xFF\?\?\?\?\?\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x8B\?\?\?\?\?\x48\x8D\x0D\?\?\?\?\xE9\?\?\?\?\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x8D\x0D\?\?\?\?\xE9\?\?\?\?\xCC\xCC\xCC\xCC\x48\x83\xEC\x28" };
	}
};