#include "CheatManager.h"

MyD3D11 g_myD3d11;

HRESULT hkPresent(IDXGISwapChain* pChain, UINT pSyncInterval, UINT pFlags)
{
	if (!g_myD3d11.InitDraw(pChain))
		return g_myD3d11.t_presentGateway(pChain, pSyncInterval, pFlags);

	CheatManager::Run();

	return g_myD3d11.t_presentGateway(pChain, pSyncInterval, pFlags);
}

bool CheatManager::InitHook()
{
	ConsoleCheatMenu::InitConsole();

	// Get original Present address with the dummy device method
	if (!g_myD3d11.SetO_Present()) return false;

	// Hook original Present address
	TrampHook tHook(
		reinterpret_cast<intptr_t*>(g_myD3d11.o_Present),
		reinterpret_cast<intptr_t*>(hkPresent),
		14); // Present stolen bytes size

	if (!tHook.IsHooked()) return false;

	g_myD3d11.t_presentGateway = reinterpret_cast<MyD3D11::T_Present>(tHook.GetGateway());

	while (!(GetAsyncKeyState(VK_DELETE) & 1)) Sleep(5);

	return true;
}

bool CheatManager::Run()
{
	if (CheatHKeys::IsOptionChanged())
		ConsoleCheatMenu::PrintCheatOptions();

	if (GameState::IsMatchStarted())
	{
		// Removing TeamCheck by default when in DeathMatch
		if(!CheatHKeys::bInitTeamCheck && CheatHKeys::SetTeamCheckDefaultValue(GameState::IsDeathMatch()))
			ConsoleCheatMenu::PrintCheatOptions();

		// Cheat Features
		if (CheatHKeys::bAimbot) Aimbot::Start();
		if (CheatHKeys::bESP)    ESP::Start();
	}
	else
	{
		CheatHKeys::bInitTeamCheck = false;
		//TODO: ESP->draw the box on myself
	}

	return true;
}