#include "GameOffset.h"

int GameOffset::GetPatternLen(char* pPattern)
{
	int count = 0;
	while (pPattern[count] != '\0' && pPattern[count + 1]  != '\0')
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

char* GameOffset::ScanModuleRegion(char* pPattern, char* pMask, char* pSrc, intptr_t pSrcSize)
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

void GameOffset::GetGamePointers(char* pPattern, char* pMask, wchar_t* pModName)
{
	char* modBase{ (char*)GetModuleHandleW(pModName) };

	if (modBase)
	{
		PIMAGE_NT_HEADERS ntHeaders = ImageNtHeader(modBase);
		if (ntHeaders != NULL)
		{
			PIMAGE_SECTION_HEADER lastSection = IMAGE_FIRST_SECTION(ntHeaders);
			ULONG sectionCount = ntHeaders->FileHeader.NumberOfSections;
			for (ULONG i = 0; i < sectionCount - 1; i++)
			{
				lastSection++;
				// Find the last section of the module
			}
			ULONG_PTR moduleSize = (ULONG_PTR)lastSection->VirtualAddress + lastSection->SizeOfRawData;
			// moduleSize now contains the size of the loaded module's image in memory

			if (moduleSize)
			{
				char* patternMatch{ ScanModuleRegion(pPattern, pMask, (char*)modBase, (intptr_t)(moduleSize)) };
				if(patternMatch)
					std::cout << patternMatch;
			}
		}
	}
}