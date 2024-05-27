#include <Windows.h>
#include "CheatManager.h"
#include "PatternScan.h"
#include "ConsoleCheatMenu.h"

extern MyD3D11 g_myD3d11;

DWORD WINAPI MainThread(HMODULE hModule)
{
	PatternScan patternScan{};

	if (!patternScan.InitPointers())
	{
		ConsoleCheatMenu::PrintErrorPtrInit(patternScan.GetPtrState());
		while (!(GetAsyncKeyState(VK_DELETE) & 1)) Sleep(10);
	}
	else if (!CheatManager::InitHook())
	{
		std::cerr << "[!] Hook failed \n";
		while (!(GetAsyncKeyState(VK_DELETE) & 1)) Sleep(10);
	}

	// Cleaning
	Sleep(500);
	ConsoleCheatMenu::DestroyConsole();
	VirtualFree(g_myD3d11.t_presentGateway, 0, MEM_RELEASE);
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	HANDLE hMainThread{};

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		hMainThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&MainThread, hModule, 0, nullptr);
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		if (hMainThread) CloseHandle(hMainThread);
	}

	return TRUE;
}