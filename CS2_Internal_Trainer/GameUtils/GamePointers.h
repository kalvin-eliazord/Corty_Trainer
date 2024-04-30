#pragma once
#include "Offset.h"
#include "Pointer.h"
#include <Windows.h>
#include <Psapi.h>
#include <cstdint>
#include <map>
#include <string>

class GamePointers
{
private:
	std::map<std::string, intptr_t> pointersState;
	bool arePointersInit{};
	bool ArePointersValid();
	void SetGameTypeIdPtr(HMODULE hModule);
	void SetViewAnglesPtr(HMODULE hModule);
	void SetGameStateIdPtr(HMODULE hModule);
	void SetCGameEntityPtr(HMODULE hModule);
	void SetSteamOverlayPtr(HMODULE hModule);
	void SetEntListBaseAddrPtr(HMODULE hModule);
	void SetLocalPlayerContPtr(HMODULE hModule);

	SIZE_T GetModuleSize(HMODULE pModule);
	int GetNumberHex(const char* pPattern);
	intptr_t* GetPatternPointer(intptr_t* pPatternMatch);
	intptr_t* GetPatternMatch(const char* pPattern, const HMODULE pModName);
	intptr_t* ScanRegion(const char* pPattern, char* pSrc, intptr_t pRegionSize);
	intptr_t* SearchGoodModRegion(const char* pPattern, char* pSrc, size_t pSrcSize);
public:
	void InitializePointers();
	bool InitGameTypeIdPtr();
	bool GetArePointersInit();
	void Set_bPointersInit(bool p_bInGamePtrInit);
	std::map<std::string, intptr_t> GetPointersState();
};

namespace Signature
{
	static constexpr char ViewAngles[]{ "\x48\x8B\x0D\?\?\?\?\x48\x8B\x01\x48\xFF\x60\x30" };
	static constexpr char CGameEntity[]{ "\x48\x8B\x0D\?\?\?\?\x48\x89\x7C\x24\?\x8B\xFA\xC1\xEB" };
	static constexpr char LocalPlayerController[]{ "\x48\x8B\x05\?\?\?\?\x48\x85\xC0\x74\?\x8B\x88" };
	static constexpr char InputSystem[]{ "\x48\x8B\?\?\?\?\?\x48\x8D\?\?\?\x45\x33\?\xE8\?\?\?\?\xF2\x0F" };
	static constexpr char CPrediction[]{ "\x48\x8D\?\?\?\?\?\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\?\x8B\x0D" };
	static constexpr char SteamOverlay[]{ "\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x41\x56\x48\x83\xEC\x20\x41\x8B\xE8\x8B\xF2\x4C\x8B\xF1\x41\xF6\xC0\x01" };
	static constexpr char WeaponList[]{ "\x48\x89\x0D\?\?\?\?\x48\x83\xC4\x28\x48\xFF\?\?\?\?\?\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x8B\?\?\?\?\?\x48\x8D\x0D\?\?\?\?\xE9\?\?\?\?\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x8D\x0D\?\?\?\?\xE9\?\?\?\?\xCC\xCC\xCC\xCC\x48\x83\xEC\x28" };
}