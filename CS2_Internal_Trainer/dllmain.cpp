#include <Windows.h>
#include "CheatManager.h"
#include "PatternScan.h"
#include "ConsoleCheatMenu.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
	PatternScan patternScan{};

	if (!(patternScan.InitPointers() && CheatManager::InitHook()))
	{
		ConsoleCheatMenu::PrintErrorPtrInit(patternScan.GetPtrState());

		while (!(GetAsyncKeyState(VK_DELETE) & 1)) Sleep(10);
	}

	ConsoleCheatMenu::DestroyConsole();
	FreeLibraryAndExitThread(hModule, 0);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);

		HANDLE hMainThread{ CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&MainThread, hModule, NULL, nullptr) };

		if (hMainThread) CloseHandle(hMainThread);
	}

	return TRUE;
}