#include "CheatManager.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
	// Initialize game pointers used by the cheat
	GameChecker::bGamePointerInit = (GamePointer::InitializePointers() ? true : false);
	
	if (GameChecker::bGamePointerInit)
		GameChecker::bGamePointerInit = CheatManager::Start();

	if (!GameChecker::bGamePointerInit)
	{
		ConsoleManager::PrintErrorPtrInit(GamePointer::pointersValue);

		while (!GetAsyncKeyState(VK_DELETE) & 1) Sleep(5);
	}

	ConsoleManager::DestroyConsole();

	FreeLibraryAndExitThread(hModule, 0);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);

		HANDLE hMainThread{ CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&MainThread, hModule, NULL, nullptr) };

		if (hMainThread)
			CloseHandle(hMainThread);
	}

	return TRUE;
}