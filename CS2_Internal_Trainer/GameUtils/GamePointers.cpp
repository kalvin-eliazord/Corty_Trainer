#include "GamePointers.h"

int GamePointers::GetNumberHex(const char* pPattern)
{
	int count{ 0 };

	while (*pPattern != '\0' || *pPattern == '\?')
	{
		++count;
		++pPattern;
	}

	return count;
}

intptr_t* GamePointers::ScanRegion(const char* pPattern, char* pSrc, intptr_t pRegionSize)
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

intptr_t* GamePointers::SearchGoodModRegion(const char* pPattern, char* pSrc, size_t pSrcSize)
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

SIZE_T GamePointers::GetModuleSize(HMODULE pModule)
{
	SIZE_T moduleSize{ NULL };

	MODULEINFO moduleInfo;
	GetModuleInformation(GetCurrentProcess(), pModule, &moduleInfo, sizeof(moduleInfo));
	moduleSize = moduleInfo.SizeOfImage;

	return moduleSize;
}

intptr_t* GamePointers::GetPatternPointer(intptr_t* pPatternMatch)
{
	// In order to extract the offset only we need to add 3 bytes
	const int32_t patternOffset{ *reinterpret_cast<int_least32_t*>(reinterpret_cast<intptr_t>(pPatternMatch) + 3) };

	// In order to access the pointer base Address we need to add 7 bytes
	intptr_t* patternPointer{ reinterpret_cast<intptr_t*>(reinterpret_cast<intptr_t>(pPatternMatch) + patternOffset + 7) };

	return patternPointer;
}

intptr_t* GamePointers::GetPatternMatch(const char* pPattern, const HMODULE pModule)
{
	SIZE_T moduleSize{ GetModuleSize(pModule) };
	if (!moduleSize) return nullptr;

	intptr_t* patternMatch{ SearchGoodModRegion(pPattern, reinterpret_cast<char*>(pModule), moduleSize) };

	return patternMatch;
}

void GamePointers::SetSteamOverlayPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::SteamOverlay, hModule) };

		if (patternMatch)
			Pointer::steamOverlay = patternMatch;
	}

	pointersState["SteamOverlay"] = reinterpret_cast<intptr_t>(Pointer::steamOverlay);
}

void GamePointers::SetGameTypeIdPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::WeaponList, hModule) };

		if (patternMatch)
		{
			intptr_t* weaponListBasePtr{ *reinterpret_cast<intptr_t**>(GetPatternPointer(patternMatch)) };

			if (weaponListBasePtr)
				Pointer::gameTypeId = reinterpret_cast<int16_t*>((reinterpret_cast<intptr_t>(weaponListBasePtr) + Offset::GameTypeId));
		}
	}

	pointersState["GameTypeIdPtr"] = reinterpret_cast<intptr_t>(Pointer::gameTypeId);
}

void GamePointers::SetGameStateIdPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::CPrediction, hModule) };

		if (patternMatch)
		{
			intptr_t* cPredictionBasePtr{ GetPatternPointer(patternMatch) };

			if (cPredictionBasePtr)
				Pointer::gameStateId = reinterpret_cast<int16_t*>(reinterpret_cast<intptr_t>(cPredictionBasePtr) + Offset::GameStateId);
		}
	}

	pointersState["GameStateId"] = reinterpret_cast<intptr_t>(Pointer::gameStateId);
}

void GamePointers::SetViewAnglesPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::InputSystem, hModule) };

		if (patternMatch)
		{
			intptr_t* inputSystemBasePtr{ *reinterpret_cast<intptr_t**>(GetPatternPointer(patternMatch)) };

			if (inputSystemBasePtr)
			{
				Pointer::lp_Pitch = reinterpret_cast<float*>((reinterpret_cast<intptr_t>(inputSystemBasePtr) + Offset::Lp_Pitch));
				Pointer::lp_Yaw = reinterpret_cast<float*>((reinterpret_cast<intptr_t>(inputSystemBasePtr) + Offset::Lp_Yaw));
			}
		}
	}
	pointersState["Pitch"] = reinterpret_cast<intptr_t>(Pointer::lp_Pitch);
	pointersState["Yaw"] = reinterpret_cast<intptr_t>(Pointer::lp_Yaw);
}

void GamePointers::SetLocalPlayerContPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::LocalPlayerController, hModule) };

		if (patternMatch)
			Pointer::lp_Controller = GetPatternPointer(patternMatch);
	}

	pointersState["LP_Controller"] = reinterpret_cast<intptr_t>(Pointer::lp_Controller);
}

void GamePointers::SetEntListBaseAddrPtr(HMODULE hModule)
{
	if (hModule)
	{
		if (Pointer::cGameEntity)
			Pointer::entityListBase = *reinterpret_cast<intptr_t**>(reinterpret_cast<intptr_t>(Pointer::cGameEntity) + Offset::EntityList);
	}

	pointersState["EntityList"] = reinterpret_cast<intptr_t>(Pointer::entityListBase);
}

void GamePointers::SetCGameEntityPtr(HMODULE hModule)
{
	if (hModule)
	{
		intptr_t* patternMatch{ GetPatternMatch(Signature::CGameEntity, hModule) };

		if (patternMatch)
			Pointer::cGameEntity = *reinterpret_cast<intptr_t**>(GetPatternPointer(patternMatch));
	}

	pointersState["CGameEntity"] = reinterpret_cast<intptr_t>(Pointer::cGameEntity);
}

void GamePointers::InitializePointers()
{
	const HMODULE hClientMod{ GetModuleHandleW(L"client.dll") };

	SetCGameEntityPtr(hClientMod);

	SetEntListBaseAddrPtr(hClientMod);

	SetLocalPlayerContPtr(hClientMod);

	SetViewAnglesPtr(hClientMod);

	const HMODULE hInputMod{ GetModuleHandleW(L"inputsystem.dll") };

	SetGameStateIdPtr(hClientMod);

	const HMODULE hGameOver{ GetModuleHandleW(L"gameoverlayrenderer64.dll") };

	SetSteamOverlayPtr(hGameOver);

	arePointersInit = ArePointersValid() ? true : false;
}

bool GamePointers::ArePointersValid()
{
	if (pointersState.empty()) return false;

	for (auto pointer : pointersState)
	{
		if (!pointer.second)
			return false;
	}

	return true;
}

bool GamePointers::InitGameTypeIdPtr()
{
	if (Pointer::gameTypeId) return true;

	const HMODULE hClientMod{ GetModuleHandleW(L"client.dll") };

	SetGameTypeIdPtr(hClientMod);

	if (!ArePointersValid()) return false;

	return true;
}

std::map<std::string, intptr_t> GamePointers::GetPointersState()
{
	return pointersState;
}

bool GamePointers::GetArePointersInit()
{
	return arePointersInit;
}

void GamePointers::Set_bPointersInit(bool p_bInGamePtrInit)
{
	arePointersInit = p_bInGamePtrInit;
}