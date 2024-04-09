#include "CheatManager.h"

bool CheatManager::Start()
{
	ConsoleManager::InitConsole();
	ConsoleManager::PrintCheatOptions();

	while (!GetAsyncKeyState(VK_DELETE) & 1)
	{
		if (AimbotOptions::IsOptionChanged())
			ConsoleManager::PrintCheatOptions();

		// IN game
		if (*GamePointer::gameStateIdPtr == GameChecker::inGameId)
		{
			if (!GamePointer::InitGameTypeIdPtr()) return false;

			if (AimbotOptions::bAimbot) Aimbot::Start();
		}

		Sleep(5);
	}

	return true;
}