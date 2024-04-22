#include "CheatManager.h"

static MyD3d11 gMyD3d11;

HRESULT __stdcall hkPresent(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags)
{
	if (CheatHKeys::bESP)
	{
		if (!gMyD3d11.mDevice || gMyD3d11.mSwapChain != pChain)
			gMyD3d11.InitD3DDraw(pChain);

		//enable this to test or debug viewport
		gMyD3d11.TestRender();
	}

	return gMyD3d11.tPresentGateway(pChain, SyncInterval, Flags);
}

bool CheatManager::Start()
{
	ConsoleManager::InitConsole();
	ConsoleManager::PrintCheatOptions();

	if (!gMyD3d11.Set_oPresent()) return false;

	TrampHook tHook(
		GamePointer::SteamOverlayPtr, //gMyD3d11.oPresent
		reinterpret_cast<intptr_t*>(hkPresent),
		28);

	gMyD3d11.tPresentGateway = reinterpret_cast<MyD3d11::TPresent>(tHook.GetGateway());

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