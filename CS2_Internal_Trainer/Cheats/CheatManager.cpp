#include "CheatManager.h"

static MyD3d11 g_myD3d11;

HRESULT hkPresent(IDXGISwapChain* pChain, UINT pSyncInterval, UINT pFlags)
{

	if (!g_myD3d11.m_device || g_myD3d11.m_swapChain != pChain)
	{
		g_myD3d11.bIsDrawInit = g_myD3d11.InitDraw(pChain);
	}
	else if (g_myD3d11.bIsDrawInit)
	{
		g_myD3d11.BeginDraw();

		g_myD3d11.TestRender();
	}

	return g_myD3d11.t_presentGateway(pChain, pSyncInterval, pFlags);
}

bool CheatManager::Start()
{
	ConsoleManager::InitConsole();
	ConsoleManager::PrintCheatOptions();

	g_myD3d11.SetPresent();

	TrampHook tHook(
		reinterpret_cast<intptr_t*>(g_myD3d11.o_present),
		reinterpret_cast<intptr_t*>(hkPresent),
		14);   //stolen bytes size

	g_myD3d11.t_presentGateway = reinterpret_cast<MyD3d11::TPresent>(tHook.GetGateway());

	Sleep(1500); // Prevent crash when game pointers aren't loaded yet

	GamePointers gamePointers{};

	while (!(GetAsyncKeyState(VK_DELETE) & 1))
	{
		if (CheatHKeys::IsOptionChanged())
			ConsoleManager::PrintCheatOptions();

		// IN game
		if (*Pointer::gameStateId == GameState::inGameId)
		{
			if (!gamePointers.InitGameTypeIdPtr()) return false;

			if (CheatHKeys::bAimbot) AimbotManager::Start();
		}

		Sleep(5);
	}

	return true;
}