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

void GamePointer::GetSteamOverlayPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::SteamOverlay, hModule) };

		if (patternMatch)
			SteamOverlayPtr = patternMatch;
	}

	pointersValue["SteamOverlay"] = reinterpret_cast<intptr_t>(SteamOverlayPtr);
}

void GamePointer::GetGameTypeIdPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::WeaponList, hModule) };

		if (patternMatch)
		{
			intptr_t* weaponListBasePtr{ *reinterpret_cast<intptr_t**>(GetPatternPointer(patternMatch)) };

			if (weaponListBasePtr)
				gameTypeIdPtr = reinterpret_cast<int16_t*>((reinterpret_cast<intptr_t>(weaponListBasePtr) + Offset::GameTypeId));
		}
	}

	pointersValue["GameTypeIdPtr"] = reinterpret_cast<intptr_t>(gameTypeIdPtr);
}

void GamePointer::GetGameStateIdPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::CPrediction, hModule) };

		if (patternMatch)
		{
			intptr_t* cPredictionBasePtr{ GetPatternPointer(patternMatch) };

			if (cPredictionBasePtr)
				gameStateIdPtr = reinterpret_cast<int16_t*>(reinterpret_cast<intptr_t>(cPredictionBasePtr) + Offset::GameStateId);
		}
	}

	pointersValue["GameStateId"] = reinterpret_cast<intptr_t>(gameStateIdPtr);
}

void GamePointer::GetViewAnglesPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::InputSystem, hModule) };

		if (patternMatch)
		{
			intptr_t* inputSystemBasePtr{ *reinterpret_cast<intptr_t**>(GetPatternPointer(patternMatch)) };

			if (inputSystemBasePtr)
			{
				lp_Pitch_Input = reinterpret_cast<float*>((reinterpret_cast<intptr_t>(inputSystemBasePtr) + Offset::Lp_Pitch));
				lp_Yaw_Input = reinterpret_cast<float*>((reinterpret_cast<intptr_t>(inputSystemBasePtr) + Offset::Lp_Yaw));
			}
		}
	}
	pointersValue["Pitch"] = reinterpret_cast<intptr_t>(lp_Pitch_Input);
	pointersValue["Yaw"] = reinterpret_cast<intptr_t>(lp_Yaw_Input);
}

void GamePointer::GetLocalPlayerContPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::LocalPlayerController, hModule) };

		if (patternMatch)
			localPlayerContPtr = GetPatternPointer(patternMatch);
	}

	pointersValue["LP_Controller"] = reinterpret_cast<intptr_t>(localPlayerContPtr);
}

void GamePointer::GetEntListBaseAddrPtr(HMODULE hModule)
{
	if (hModule)
	{
		if (CGameEntityPtr)
			entityListBasePtr = *reinterpret_cast<intptr_t**>(reinterpret_cast<intptr_t>(CGameEntityPtr) + Offset::EntityList);
	}

	pointersValue["EntityList"] = reinterpret_cast<intptr_t>(entityListBasePtr);
}

void GamePointer::GetCGameEntityPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::CGameEntity, hModule) };

		if (patternMatch)
			CGameEntityPtr = *reinterpret_cast<intptr_t**>(GetPatternPointer(patternMatch));
	}

	pointersValue["CGameEntity"] = reinterpret_cast<intptr_t>(CGameEntityPtr);
}

bool GamePointer::InitializePointers()
{
	const HMODULE hClientMod{ GetModuleHandleW(L"client.dll") };

	GetCGameEntityPtr(hClientMod);

	GetEntListBaseAddrPtr(hClientMod);

	GetLocalPlayerContPtr(hClientMod);

	GetViewAnglesPtr(hClientMod);

	const HMODULE hInputMod{ GetModuleHandleW(L"inputsystem.dll") };

	GetGameStateIdPtr(hClientMod);

	const HMODULE hGameOver{ GetModuleHandleW(L"gameoverlayrenderer64.dll") };

	GetSteamOverlayPtr(hGameOver);

	if(!CheckPointers()) return false;

	return true;
}

bool GamePointer::CheckPointers()
{
	if (pointersValue.empty()) return false;

	for (auto pointer : pointersValue)
	{
		if (!pointer.second)
			return false;
	}

	return true;
}

bool GamePointer::InitGameTypeIdPtr()
{
	if (gameTypeIdPtr) return true;

	const HMODULE hClientMod{ GetModuleHandleW(L"client.dll") };

	GetGameTypeIdPtr(hClientMod);

	if (!CheckPointers) return false;

	return true;
}