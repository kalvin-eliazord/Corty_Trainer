#include <windows.h>
#include "GamePointer.h"
#include "ConsoleManager.h"
#include "CheatManager.h"

int32_t WINAPI MainThread(HMODULE hModule)
{
	// Init game pointers used for cheats
	if (GamePointer::InitializePointers())
	{
		ConsoleManager::InitConsole();

		while (!GetAsyncKeyState(VK_DELETE) & 1)
		{
			if (GetAsyncKeyState(VK_F9) & 1)
			{
				CheatManager::bAimbot = !CheatManager::bAimbot;
				ConsoleManager::PrintCheatOptions();
			}

			if (CheatManager::bAimbot)
				CheatManager::StartAimbot();

			Sleep(5);
		}
	}

	ConsoleManager::DestroyConsole();

	FreeLibraryAndExitThread(hModule, 0);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	int32_t  ul_reason_for_call,
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