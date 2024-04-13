#include "GamePointer.h"

intptr_t* GamePointer::GetPatternPointer(intptr_t* pPatternMatch)
{
	// In order to extract the offset only we need to add 3 bytes
	const int_least32_t patternOffset{ *reinterpret_cast<int_least32_t*>(reinterpret_cast<intptr_t>(pPatternMatch) + 3) };

	// In order to access the pointer base Address we need to add 7 bytes
	intptr_t* patternPointer{ reinterpret_cast<intptr_t*>(reinterpret_cast<intptr_t>(pPatternMatch) + patternOffset + 7) };

	return patternPointer;
}

int GamePointer::GetNumberHex(const char* pPattern)
{
	int count{ 0 };
	while (*pPattern != '\0' || *pPattern == '\?')
	{
		++count;
		++pPattern;
	}

	return count;
}

intptr_t* GamePointer::ScanRegion(const char* pPattern, char* pSrc, intptr_t pRegionSize)
{
	const int patternLen{ GetNumberHex(pPattern) };

	for (int i{ 0 }; i < pRegionSize; ++i)
	{
		bool bFound{ true };

		for (int j{ 0 }; j < patternLen; ++j)
		{
			// Searching for signature match
			if (pPattern[j] != '\?' && pPattern[j] != *(pSrc + i + j))
			{
				bFound = false;
				break;
			}
		}

		if (bFound)
			return reinterpret_cast<intptr_t*>(pSrc + i);
	}
	return NULL;
}

intptr_t* GamePointer::SearchGoodModRegion(const char* pPattern, char* pSrc, size_t pSrcSize)
{
	MEMORY_BASIC_INFORMATION mbi{};

	for (char* currRegion{ pSrc }; currRegion < pSrc + pSrcSize; currRegion += mbi.RegionSize)
	{
		// Filtering only readable region
		if (!VirtualQuery(currRegion, &mbi, sizeof(mbi)) || mbi.Protect == PAGE_NOACCESS || mbi.State != MEM_COMMIT)
			continue;

		intptr_t* patternMatch = { ScanRegion(pPattern, currRegion, mbi.RegionSize) };

		if (patternMatch) return patternMatch;
	}

	return NULL;
}

SIZE_T GamePointer::GetModuleSize(HMODULE pModule)
{
	SIZE_T moduleSize{ NULL };

	MODULEINFO moduleInfo;
	GetModuleInformation(GetCurrentProcess(), pModule, &moduleInfo, sizeof(moduleInfo));
	moduleSize = moduleInfo.SizeOfImage;

	return moduleSize;
}

intptr_t* GamePointer::GetPatternMatch(const char* pPattern, const HMODULE pModule)
{
	SIZE_T moduleSize{ GetModuleSize(pModule) };
	if (!moduleSize) return nullptr;

	intptr_t* patternMatch{ SearchGoodModRegion(pPattern, reinterpret_cast<char*>(pModule), moduleSize) };

	return patternMatch;
}

bool GamePointer::GetSteamOverlayPtr(HMODULE hModule)
{
	intptr_t* patternMatch{ GetPatternMatch(Signature::SteamOverlay, hModule) };
	if (!patternMatch) return false;

	SteamOverlayPtr = patternMatch;

	return true;
}

bool GamePointer::GetGameTypeIdPtr(HMODULE hModule)
{
	intptr_t* patternMatch{ GetPatternMatch(Signature::WeaponList, hModule) };
	if (!patternMatch) return false;

	intptr_t* weaponListBasePtr{ *reinterpret_cast<intptr_t**>(GetPatternPointer(patternMatch)) };
	if (!weaponListBasePtr) return false;

	gameTypeIdPtr = reinterpret_cast<int_least8_t*>((reinterpret_cast<intptr_t>(weaponListBasePtr) + Offset::gameTypeId));
	if (!gameTypeIdPtr) return false;

	return true;
}

bool GamePointer::GetGameStateIdPtr(HMODULE hModule)
{
	intptr_t* patternMatch{ GetPatternMatch(Signature::CPrediction, hModule) };
	if (!patternMatch) return false;

	intptr_t* cPredictionBasePtr{ GetPatternPointer(patternMatch) };
	if (!cPredictionBasePtr) return false;

	gameStateIdPtr = reinterpret_cast<int_least8_t*>(reinterpret_cast<intptr_t>(cPredictionBasePtr) + Offset::gameStateId);
	if (!gameStateIdPtr) return false;

	return true;
}

bool GamePointer::GetViewAnglesPtr(HMODULE hModule)
{
	intptr_t* patternMatch{ GetPatternMatch(Signature::InputSystem, hModule) };
	if (!patternMatch) return false;

	intptr_t* inputSystemBasePtr{ *reinterpret_cast<intptr_t**>(GetPatternPointer(patternMatch)) };
	if (!inputSystemBasePtr) return false;

	lp_Pitch_Input = reinterpret_cast<float*>((reinterpret_cast<intptr_t>(inputSystemBasePtr) + Offset::lp_Pitch));
	if (!lp_Pitch_Input) return false;

	lp_Yaw_Input = reinterpret_cast<float*>((reinterpret_cast<intptr_t>(inputSystemBasePtr) + Offset::lp_Yaw));
	if (!lp_Yaw_Input) return false;

	return true;
}

bool GamePointer::GetLocalPlayerPawnPtr(HMODULE hModule)
{
	intptr_t* patternMatch{ GetPatternMatch(Signature::CPrediction, hModule) };
	if (!patternMatch) return false;

	intptr_t* cPredictionBasePtr{ GetPatternPointer(patternMatch) };
	if (!cPredictionBasePtr) return false;

	localPlayerPawnPtr = *reinterpret_cast<intptr_t**>(reinterpret_cast<intptr_t>(cPredictionBasePtr) + Offset::lp_Pawn);
	if (!localPlayerPawnPtr) return false;

	return true;
}

bool GamePointer::GetLocalPlayerContPtr(HMODULE hModule)
{
	intptr_t* patternMatch{ GetPatternMatch(Signature::LocalPlayerController, hModule) };
	if (!patternMatch) return false;

	localPlayerContPtr = GetPatternPointer(patternMatch);
	if (!localPlayerContPtr) return false;

	return true;
}

bool GamePointer::GetEntListBaseAddrPtr(HMODULE hModule)
{
	intptr_t* patternMatch{ GetPatternMatch(Signature::EntityList, hModule) };
	if (!patternMatch) return false;

	intptr_t* cGameEntityBaseAddrPtr{ *reinterpret_cast<intptr_t**>(GetPatternPointer(patternMatch)) };
	if (!cGameEntityBaseAddrPtr) return false;

	entityListBasePtr = *reinterpret_cast<intptr_t**>(reinterpret_cast<intptr_t>(cGameEntityBaseAddrPtr) + Offset::entityList);
	if (!entityListBasePtr) return false;

	return true;
}

bool GamePointer::InitializePointers()
{
	const HMODULE hClientMod{ GetModuleHandleW(L"client.dll") };
	if (!hClientMod) return false;

	if (!GetEntListBaseAddrPtr(hClientMod)) return false;

	if (!GetLocalPlayerContPtr(hClientMod)) return false;

	if (!GetViewAnglesPtr(hClientMod)) return false;

	const HMODULE hInputMod{ GetModuleHandleW(L"inputsystem.dll") };
	if (!hInputMod) return false;

	if (!GetGameStateIdPtr(hClientMod)) return false;

	const HMODULE hGameOver{ GetModuleHandleW(L"gameoverlayrenderer64.dll") };
	if (!hGameOver) return false;

	if (!GetSteamOverlayPtr(hGameOver)) return false;

	return true;
}

bool GamePointer::InitGameTypeIdPtr()
{
	if (gameTypeIdPtr) return true;

	const HMODULE hClientMod{ GetModuleHandleW(L"client.dll") };
	if (!hClientMod) return false;

	if (!GetGameTypeIdPtr(hClientMod)) return false;

	if (!GetLocalPlayerPawnPtr(hClientMod)) return false;

	return true;
}