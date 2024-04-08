#include "GamePointer.h"

intptr_t* GamePointer::GetPointerBaseAddress(intptr_t* pSignatureMatch)
{
	// In order to extract the offset only we need to add 3 bytes
	const int32_t signatureOffset{ *reinterpret_cast<int_least32_t*>(reinterpret_cast<intptr_t>(pSignatureMatch) + 3) };

	// In order to access the pointer base Address we need to add 7 bytes
	intptr_t* pointerBaseAddr{ reinterpret_cast<intptr_t*>(reinterpret_cast<intptr_t>(pSignatureMatch) + signatureOffset + 7) };
	
	return pointerBaseAddr;
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

intptr_t* GamePointer::CompareSignatureWithBytes(const char* pPattern, char* pSrc, intptr_t pRegionSize)
{
	const int patternLen{ GetNumberHex(pPattern)};

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

intptr_t* GamePointer::ScanModuleRegion(const char* pPattern, char* pSrc, size_t pSrcSize)
{
	MEMORY_BASIC_INFORMATION mbi{};

	for (char* currRegion{ pSrc }; currRegion < pSrc + pSrcSize; currRegion += mbi.RegionSize)
	{
		// Filtering only readable region
		if (!VirtualQuery(currRegion, &mbi, sizeof(mbi)) || mbi.Protect == PAGE_NOACCESS || mbi.State != MEM_COMMIT)
			continue;

		intptr_t* patternMatch = { CompareSignatureWithBytes(pPattern, currRegion, mbi.RegionSize) };

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

intptr_t* GamePointer::GetSignatureResult(const char* pPattern, const HMODULE pModule)
{
	SIZE_T moduleSize{ NULL };
	intptr_t* signatureMatch{ nullptr };

	moduleSize = GetModuleSize(pModule);

	if (moduleSize)
		signatureMatch = ScanModuleRegion(pPattern, reinterpret_cast<char*>(pModule), moduleSize);

	if (!signatureMatch) return nullptr;

	return GetPointerBaseAddress(signatureMatch);
}

bool GamePointer::InitializePointers()
{
	const HMODULE hClientMod{ GetModuleHandleW(L"client.dll") };
	if (!hClientMod) return false;
	
	intptr_t* cGameEntityBaseAddrPtr{ *reinterpret_cast<intptr_t**>(GetSignatureResult(Signature::entityList, hClientMod)) };
	if (!cGameEntityBaseAddrPtr) return false;

	entityListBasePtr = *reinterpret_cast<intptr_t**>(reinterpret_cast<intptr_t>(cGameEntityBaseAddrPtr) + 0x10);
	if (!entityListBasePtr) return false;

	localPlayerPtr = GetSignatureResult(Signature::LocalPlayerController, hClientMod);
	if (!localPlayerPtr) return false;

	intptr_t* lpInputBasePtr{ *reinterpret_cast<intptr_t**>(GetSignatureResult(Signature::LpInputBase, hClientMod)) };
	if (!lpInputBasePtr) return false;

	lp_Pitch_Input = reinterpret_cast<float*>((reinterpret_cast<intptr_t>(lpInputBasePtr) + Offset::lp_Pitch));
	if (!lp_Pitch_Input) return false;

	lp_Yaw_Input = reinterpret_cast<float*>((reinterpret_cast<intptr_t>(lpInputBasePtr) + Offset::lp_Yaw));
	if (!lp_Yaw_Input) return false;

	const HMODULE hInputMod{ GetModuleHandleW(L"inputsystem.dll") };
	if (!hInputMod) return false;

	intptr_t* inputBaseAddrPtr{ GetSignatureResult(Signature::InputSystem, hInputMod)};
	if (!inputBaseAddrPtr) return false;

	intptr_t* cPredictionBasePtr{ *reinterpret_cast<intptr_t**>(reinterpret_cast<intptr_t>(inputBaseAddrPtr) - Offset::cPrediction) };
	if (!cPredictionBasePtr) return false;

	gameStateIdPtr = reinterpret_cast<int_least8_t*>(reinterpret_cast<intptr_t>(cPredictionBasePtr) + Offset::gameStateId);
	if (!gameStateIdPtr) return false;

	return true;
}

bool GamePointer::InitializePointersInGame()
{
	if (gameTypeIdPtr) return true;

	const HMODULE hClientMod{ GetModuleHandleW(L"client.dll") };
	if (!hClientMod) return false;

	intptr_t* weaponListBasePtr{ *reinterpret_cast<intptr_t**>(GetSignatureResult(Signature::WeaponList, hClientMod)) };
	if (!weaponListBasePtr) return false;

	gameTypeIdPtr = reinterpret_cast<int_least8_t*>((reinterpret_cast<intptr_t>(weaponListBasePtr) + Offset::gameTypeId));
	if (!gameTypeIdPtr) return false;

	return true;
}