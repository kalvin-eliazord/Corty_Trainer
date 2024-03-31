#include <windows.h>
#include "GamePointer.h"
#include "ConsoleManager.h"
#include "CheatManager.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
	// Init a usable console
	ConsoleManager consoleManager{};

	// Init game pointers used for aimbot
	if (GamePointer::InitializePointers())
		CheatManager::StartAimbot();

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

	if (ul_reason_for_call == DLL_THREAD_DETACH)
	{
		if (hMainThread)
			CloseHandle(hMainThread);
	}

	return TRUE;
}