#include "GameOffset.h"

int GameOffset::GetPatternLen(char* pPattern)
{
	int count = 0;
	while (pPattern[count] != '\0')
		count++;

	return count;
}

char* GameOffset::GetPatternMatch(char* pPattern, char* pMask, char* pSrc, intptr_t pRegionSize)
{
	const int patternLen {GetPatternLen(pPattern)};
	
	for (int i{ 0 }; i < pRegionSize; ++i)
	{
		bool bFound{ true };

		for (int j{ 0 }; j < patternLen; ++j)
		{
			if (pMask[j] != '\?' && pPattern[j] != *(char*)((intptr_t)pSrc + i + j))
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

char* GameOffset::GetGamePointers(char* pPattern, char* pMask, wchar_t* pModName)
{
	MODULEINFO moduleInfo;
	GetModuleInformation(GetCurrentProcess(), Client::modBaseAddr, &moduleInfo, sizeof(moduleInfo));
	SIZE_T moduleSize = moduleInfo.SizeOfImage;

	char* patternMatch = ScanModuleRegion(pPattern, pMask, (char*)Client::modBaseAddr, moduleSize);

	return patternMatch;
}