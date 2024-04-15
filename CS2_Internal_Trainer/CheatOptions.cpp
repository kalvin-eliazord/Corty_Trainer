#include "CheatOptions.h"

bool CheatOptions::IsOptionChanged()
{
	if (GetAsyncKeyState(VK_F2) & 1)
	{
		CheatOptions::bTargetLock = !CheatOptions::bTargetLock;
		return true;
	}
	else if (GetAsyncKeyState(VK_F3) & 1)
	{
		--CheatOptions::smoothValue;
		return true;

	}
	else if (GetAsyncKeyState(VK_F4) & 1 &&
		CheatOptions::smoothValue > 0)
	{
		++CheatOptions::smoothValue;
		return true;
	}
	else if (GetAsyncKeyState(VK_F5) & 1 &&
		CheatOptions::fovValue > 10)
	{
		CheatOptions::fovValue -= 10;
		return true;
	}
	else if (GetAsyncKeyState(VK_F6) & 1)
	{
		CheatOptions::fovValue += 10;
		return true;
	}
	else if (GetAsyncKeyState(VK_F8) & 1)
	{
		CheatOptions::bESP = !CheatOptions::bESP;
		return true;
	}
	else if (GetAsyncKeyState(VK_F9) & 1)
	{
		CheatOptions::bAimbot = !(CheatOptions::bAimbot);
		return true;
	}

	return false;
}