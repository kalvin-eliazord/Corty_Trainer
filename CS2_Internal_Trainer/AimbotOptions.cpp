#include "AimbotOptions.h"

void AimbotOptions::OptionsCheck()
{
	if (GetAsyncKeyState(VK_F4) & 1)
	{
		++AimbotOptions::smoothValue;
		ConsoleManager::bConsoleChanged = true;
	}
	else if (GetAsyncKeyState(VK_F3) & 1 && AimbotOptions::smoothValue > 0)
	{
		--AimbotOptions::smoothValue;
		ConsoleManager::bConsoleChanged = true;
	}
	else if (GetAsyncKeyState(VK_F2) & 1)
	{
		AimbotOptions::bTargetLock = !AimbotOptions::bTargetLock;
		ConsoleManager::bConsoleChanged = true;
	}
	else if (GetAsyncKeyState(VK_F5) & 1 && AimbotOptions::fovValue > 10)
	{
		AimbotOptions::fovValue -= 10;
		ConsoleManager::bConsoleChanged = true;
	}
	else if (GetAsyncKeyState(VK_F6) & 1)
	{
		AimbotOptions::fovValue += 10;
		ConsoleManager::bConsoleChanged = true;
	}
}