#pragma once
#include "Offset.h"
#include "Pointer.h"
#include <Windows.h>
#include <Psapi.h>
#include <map>
#include <string>

class PatternScan
{
private:
	std::map<std::string, intptr_t> pointersState;
	bool ArePointersValid();
	void SetGameRulesPtr(HMODULE hModule);
	void SetViewMatrixPtr(HMODULE hModule);
	void SetViewAnglesPtr(HMODULE hModule);
	void SetGameStateIdPtr(HMODULE hModule);
	void SetCGameEntityPtr(HMODULE hModule);
	void SetEntListBaseAddrPtr(HMODULE hModule);
	void SetLocalPlayerContPtr(HMODULE hModule);

	SIZE_T GetModuleSize(HMODULE pModule);
	int GetNumberHex(const char* pPattern);
	intptr_t* GetPatternPointer(intptr_t* pPatternMatch);
	intptr_t* GetPatternMatch(const char* pPattern, const HMODULE pModName);
	intptr_t* ScanRegion(const char* pPattern, char* pSrc, intptr_t pRegionSize);
	intptr_t* SearchGoodModRegion(const char* pPattern, char* pSrc, size_t pSrcSize);
public:
	bool InitPtrs();
	std::map<std::string, intptr_t> GetPointersState();
};

namespace Signature
{
	inline constexpr char ViewMatrix[]{ "\x48\x8D\x0D\?\?\?\?\x48\xC1\xE0\x06" };
	inline constexpr char ViewAngles[]{ "\x48\x8B\x0D\?\?\?\?\x48\x8B\x01\x48\xFF\x60\x30" };
	inline constexpr char CGameEntity[]{ "\x48\x8B\x0D\?\?\?\?\x48\x89\x7C\x24\?\x8B\xFA\xC1\xEB" };
	inline constexpr char LocalPlayerController[]{ "\x48\x8B\x05\?\?\?\?\x48\x85\xC0\x74\?\x8B\x88" };
	inline constexpr char InputSystem[]{ "\x48\x8B\?\?\?\?\?\x48\x8D\?\?\?\x45\x33\?\xE8\?\?\?\?\xF2\x0F" };
	inline constexpr char CPrediction[]{ "\x48\x8D\?\?\?\?\?\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\?\x8B\x0D" };
	inline constexpr char GameRules[]{ "\x48\x8B\?\?\?\?\?\x48\x85\?\x74\?\x85\xC9\x75\?\x38\x48\?\x75\?\x38\x48\?\x74\?\xB0" };
	//inline constexpr char WeaponList[]{ "\x48\x89\x0D\?\?\?\?\x48\x83\xC4\x28\x48\xFF\?\?\?\?\?\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x8B\?\?\?\?\?\x48\x8D\x0D\?\?\?\?\xE9\?\?\?\?\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x8D\x0D\?\?\?\?\xE9\?\?\?\?\xCC\xCC\xCC\xCC\x48\x83\xEC\x28" };
}