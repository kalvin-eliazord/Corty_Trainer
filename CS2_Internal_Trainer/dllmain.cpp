#include "CheatManager.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
	// Init game pointers used for the aimbot
	GameChecker::bGamePointerInit = (GamePointer::InitializePointers() ? true : false);
	
	if (GameChecker::bGamePointerInit)
		GameChecker::bGamePointerInit = CheatManager::Start();

	if (!GameChecker::bGamePointerInit)
	{
		ConsoleManager::PrintErrorPtrInit();

		while (!GetAsyncKeyState(VK_DELETE) & 1) Sleep(5);
	}

	ConsoleManager::DestroyConsole();

	FreeLibraryAndExitThread(hModule, 0);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	HANDLE hMainThread{ nullptr };

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		hMainThread = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&MainThread, hModule, NULL, nullptr);
	else if(ul_reason_for_call == DLL_THREAD_DETACH)
	{
		if (hMainThread)
			CloseHandle(hMainThread);
	}

	return TRUE;
}