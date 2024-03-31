#include "GamePointer.h"

char* GamePointer::GetPatternMatch(char* pPattern, char* pMask, char* pSrc, intptr_t pRegionSize)
{
	const int patternLen { sizeof(pPattern)+1};
	
	for (int i{ 0 }; i < pRegionSize; ++i)
	{
		bool bFound{ true };

		for (int j{ 0 }; j < patternLen; ++j)
		{
			if (pMask[j] != '\x00' && pPattern[j] != *(char*)((intptr_t)pSrc + i + j))
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

char* GamePointer::ScanModuleRegion(char* pPattern, char* pMask, char* pSrc, size_t pSrcSize)
{
	MEMORY_BASIC_INFORMATION mbi{};

	for (char* currRegion = pSrc; currRegion < pSrc + pSrcSize; currRegion += mbi.RegionSize)
	{
		if (!VirtualQuery(currRegion, &mbi, sizeof(mbi)) || mbi.Protect == PAGE_NOACCESS || mbi.State != MEM_COMMIT)
			continue;

		char* patternMatch{ GetPatternMatch(pPattern, pMask, currRegion, mbi.RegionSize) };

		if (patternMatch != nullptr)
			return patternMatch;
	}

	return nullptr;
}

intptr_t* GamePointer::GetGamePointer(char* pPattern, wchar_t* pModName)
{
	intptr_t* patternMatch{ nullptr };
	char* mask{ pPattern };

	HMODULE modBaseAddr{ GetModuleHandleW(pModName) };

	if (modBaseAddr)
	{
		MODULEINFO moduleInfo;
		GetModuleInformation(GetCurrentProcess(), modBaseAddr, &moduleInfo, sizeof(moduleInfo));
		SIZE_T moduleSize = moduleInfo.SizeOfImage;

		patternMatch = (intptr_t*)(ScanModuleRegion(pPattern, mask, (char*)modBaseAddr, moduleSize));
	}

	return patternMatch;
}

bool GamePointer::InitializePointers()
{
	// Get signatures
	char* entityListSign{ (char*)"\x88\x4C\x00\x00\xFE\x7F\x00\x00\x20" };
	entityListPtr = *(intptr_t**)(GetGamePointer(entityListSign, (wchar_t*)L"client.dll"));
	if (!entityListPtr) return false;

	localPlayerPtr = (intptr_t*)((intptr_t)entityListPtr + Offset::lpBaseAddr);
	if (!localPlayerPtr) return false;

	 char* cPredictionSign{ (char*)"\x38\xF8\xB3\x00\xFE\x7F\x00\x00\xA0" };
	intptr_t* cPredictionBaseAddr{ GetGamePointer(cPredictionSign, (wchar_t*)L"client.dll")};
	if (!cPredictionBaseAddr) return false;

	gameStateIdPtr = (int*)((intptr_t)cPredictionBaseAddr - Offset::gameStateId);
	if (!gameStateIdPtr) return false;

	char* csGoTeamPreviewCamSign{ (char*)"\xD3\xC8\x00\x00\x00\x00\x00\x00\xD3\xC8" };
	intptr_t* csGoTeamPreviewCam{ (intptr_t*)((intptr_t)GetGamePointer(csGoTeamPreviewCamSign, (wchar_t*)L"client.dll") - 1) };
	if (!csGoTeamPreviewCam) return false;

	lp_Pitch_Input = (float*)((intptr_t)csGoTeamPreviewCam - Offset::lp_Pitch);
	if (!lp_Pitch_Input) return false;

	lp_Yaw_Input = (float*)((intptr_t)csGoTeamPreviewCam - Offset::lp_Yaw);
	if (!lp_Yaw_Input) return false;

	return true;
}

bool GamePointer::InitializePointersInGame()
{
	char* baseAddrNearGameTypeSign{ (char*)"\x50\x1A\x00\x00\xFE\x7F\x00\x00\x30\x84\xA8" };
	intptr_t* baseAddrNearGameType = GetGamePointer(baseAddrNearGameTypeSign, (wchar_t*)L"client.dll");
	if (!baseAddrNearGameType) return false;

	gameTypeIdPtr = (int8_t*)((intptr_t)baseAddrNearGameType + Offset::gameTypeId);
	if (!gameTypeIdPtr) return false;

	return true;
}
