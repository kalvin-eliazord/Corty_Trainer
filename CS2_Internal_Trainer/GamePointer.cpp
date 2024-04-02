#include "GamePointer.h"

char* GamePointer::GetPatternMatch(char* pPattern, char* pSrc, intptr_t pRegionSize)
{
	const int patternLen{ sizeof(pPattern) + 1 };

	for (int i{ 0 }; i < pRegionSize; ++i)
	{
		bool bFound{ true };

		for (int j{ 0 }; j < patternLen; ++j)
		{
			// Searching for pattern match
			if (pPattern[j] != '\x00' && pPattern[j] != *(char*)((intptr_t)pSrc + i + j))
			{
				bFound = false;
				break;
			}
		}

		if (bFound)
			return (pSrc + i);
	}
	return nullptr;
}

char* GamePointer::ScanModRegion(char* pPattern, char* pSrc, size_t pSrcSize)
{
	MEMORY_BASIC_INFORMATION mbi{};

	for (char* currRegion{ pSrc }; currRegion < pSrc + pSrcSize; currRegion += mbi.RegionSize)
	{
		// Filtering only readable region
		if (!VirtualQuery(currRegion, &mbi, sizeof(mbi)) || mbi.Protect == PAGE_NOACCESS || mbi.State != MEM_COMMIT)
			continue;

		char* patternMatch = GetPatternMatch(pPattern, currRegion, mbi.RegionSize);

		if (patternMatch != nullptr)
			return patternMatch;
	}

	return nullptr;
}

SIZE_T GamePointer::GetModuleSize(HMODULE pModule)
{
	SIZE_T moduleSize{ NULL };

	MODULEINFO moduleInfo;
	GetModuleInformation(GetCurrentProcess(), pModule, &moduleInfo, sizeof(moduleInfo));
	moduleSize = moduleInfo.SizeOfImage;

	return moduleSize;
}

intptr_t* GamePointer::GetGamePointer(char* pPattern, wchar_t* pModName)
{
	intptr_t* patternMatch{ nullptr };

	// Get Module base address and size
	HMODULE hModule{ GetModuleHandleW(pModName) };

	SIZE_T moduleSize{ NULL };
	if (hModule) moduleSize = GetModuleSize(hModule);

	if (moduleSize)
		patternMatch = (intptr_t*)ScanModRegion(pPattern, (char*)hModule, moduleSize);

	return patternMatch;
}

bool GamePointer::InitializePointers()
{
	// Get Pattern address
	char* entityListPat{ (char*)"\x88\x4C\x00\x00\x00\x7F\x00\x00\x20" };
	intptr_t* entityListBuffer = (intptr_t*)(GetGamePointer(entityListPat, (wchar_t*)L"client.dll"));
	if (!entityListBuffer) return false;

	entityListPtr = *(intptr_t**)entityListBuffer;
	if (!entityListPtr) return false;

	localPlayerPtr = (intptr_t*)((intptr_t)entityListPtr + Offset::lpBaseAddr);
	if (!localPlayerPtr) return false;

	char* cPredictionPat{ (char*)"\x38\xF8\x00\x00\x00\x7F\x00\x00\xA0" };
	intptr_t* cPredictionBaseAddr{ GetGamePointer(cPredictionPat, (wchar_t*)L"client.dll") };
	if (!cPredictionBaseAddr) return false;

	gameStateIdPtr = (int*)((intptr_t)cPredictionBaseAddr - Offset::gameStateId);
	if (!gameStateIdPtr) return false;

	char* csGoInputBaseAddrPat{ (char*)"\x78\x52\x00\x00\x00\x7F\x00\x00\x88" };
	intptr_t* csGoInputBaseAddr{ GetGamePointer(csGoInputBaseAddrPat, (wchar_t*)L"client.dll") };
	if (!csGoInputBaseAddr) return false;

	lp_Pitch_Input = (float*)((intptr_t)csGoInputBaseAddr - Offset::lp_Pitch);
	if (!lp_Pitch_Input) return false;

	lp_Yaw_Input = (float*)((intptr_t)csGoInputBaseAddr - Offset::lp_Yaw);
	if (!lp_Yaw_Input) return false;

	return true;
}

bool GamePointer::InitializePointersInGame()
{
	char* baseAddrNearGameTypePat{ (char*)"\x50\x1A\x00\x00\x00\x7F\x00\x00\x30\x84" };
	intptr_t* baseAddrNearGameType = GetGamePointer(baseAddrNearGameTypePat, (wchar_t*)L"client.dll");
	if (!baseAddrNearGameType) return false;

	gameTypeIdPtr = (int8_t*)((intptr_t)baseAddrNearGameType + Offset::gameTypeId);
	if (!gameTypeIdPtr) return false;

	return true;
}