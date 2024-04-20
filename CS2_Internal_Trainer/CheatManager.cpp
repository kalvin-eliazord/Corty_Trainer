#include "CheatManager.h"

MyD3d11 myD3d11;

HRESULT __stdcall hkPresent(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags)
{
	if (CheatHKeys::bESP)
	{
		if (!myD3d11.mDevice || myD3d11.mSwapChain != pChain)
			myD3d11.InitD3DDraw(pChain);

		//enable this to test or debug viewport
		myD3d11.TestRender();
	}

	return myD3d11.tPresentGateway(pChain, SyncInterval, Flags);
}

bool CheatManager::Start()
{
	ConsoleManager::InitConsole();
	ConsoleManager::PrintCheatOptions();

	if (!myD3d11.Set_oPresent()) return false;

	TrampHook tHook(
		GamePointer::SteamOverlayPtr, //myD3d11.oPresent
		reinterpret_cast<intptr_t*>(hkPresent),
		28);

	myD3d11.tPresentGateway = reinterpret_cast<MyD3d11::TPresent>(tHook.GetGateway());

	Sleep(1500); // Prevent crash when game values aren't loaded yet

	while (!GetAsyncKeyState(VK_DELETE) & 1)
	{
		if (CheatHKeys::IsOptionChanged())
			ConsoleManager::PrintCheatOptions();

		// IN game
		if (*GamePointer::gameStateIdPtr == GameChecker::inGameId)
		{
			if (!GamePointer::InitGameTypeIdPtr()) return false;

			if (CheatHKeys::bAimbot) AimbotManager::Start();
		}

		Sleep(5);
	}

	return true;
}