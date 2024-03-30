#include "GameOffset.h"
#include <Psapi.h>

char* GameOffset::GetPatternMatch(char* pPattern, char* pMask, char* pSrc, intptr_t pRegionSize)
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

char* GameOffset::ScanModuleRegion(char* pPattern, char* pMask, char* pSrc, size_t pSrcSize)
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

char* GameOffset::GetGamePointer(char* pPattern, wchar_t* pModName)
{
	char* mask{ pPattern };

	HMODULE modBaseAddr{ GetModuleHandleW(pModName) };

	MODULEINFO moduleInfo;
	GetModuleInformation(GetCurrentProcess(), modBaseAddr, &moduleInfo, sizeof(moduleInfo));
	SIZE_T moduleSize = moduleInfo.SizeOfImage;

	char* patternMatch = ScanModuleRegion(pPattern, mask, (char*)modBaseAddr, moduleSize);

	return patternMatch;
}

bool GameOffset::UpdatePointers()
{
	// Get signatures
	char* entityListSign{ (char*)"\x88\x4C\x00\x00\xFE\x7F\x00\x00\x20" };
	Client::entityListPtr = *(EntityList**)GetGamePointer(entityListSign, (wchar_t*)L"client.dll");

	delete[] entityListSign;
	if (!Client::entityListPtr) return false;

	Client::localPlayerPtr = *(Entity**)(Client::entityListPtr + Offset::lpBaseAddr);
	if (!Client::localPlayerPtr) return false;

	char* cPredictionSign{ (char*)"\x98\xA6\x73" };
	Client::gameStateIdPtr = (int*)(GetGamePointer(cPredictionSign, (wchar_t*)L"client.dll") + Offset::gameStateId);

	delete[] cPredictionSign;
	if (!Client::gameStateIdPtr) return false;

	char* csGoInputSign{ (char*)"\xE8\x00\x00\x00\x00\x7F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xF0\xFF\xB7" };
	Client::csGoInputBaseAddr = (intptr_t*)(GetGamePointer(csGoInputSign, (wchar_t*)L"client.dll"));

	delete[] csGoInputSign;
	if (!Client::csGoInputBaseAddr) return false;

	Client::lp_Pitch_Input = (float*)(Client::csGoInputBaseAddr + Offset::lp_Pitch);
	if (!Client::lp_Pitch_Input) return false;

	Client::lp_Yaw_Input = (float*)(Client::csGoInputBaseAddr + Offset::lp_Yaw);
	if (!Client::lp_Yaw_Input) return false;

	char* CWatchableFriendsSign{ (char*)"\x50\x1A\x7B" };
	Client::CWatchableBaseAddr = (intptr_t*)(GetGamePointer(CWatchableFriendsSign, (wchar_t*)L"client.dll"));

	delete[] CWatchableFriendsSign;
	if (!Client::CWatchableBaseAddr) return false;

	Client::gameTypeIdPtr = (int8_t*)(Client::CWatchableBaseAddr + Offset::gameTypeId);
	if (!Client::gameTypeIdPtr) return false;

	return true;
}
