#include "CheatHKeys.h"

bool CheatHKeys::IsOptionChanged()
{
	if (GetAsyncKeyState(VK_F1) & 1)
	{
		CheatHKeys::bHeadPos = !CheatHKeys::bHeadPos;
		return true;
	}
	else if (GetAsyncKeyState(VK_F2) & 1)
	{
		CheatHKeys::bTargetLock = !CheatHKeys::bTargetLock;
		return true;
	}
	else if (GetAsyncKeyState(VK_F3) & 1 &&
		CheatHKeys::smoothValue > 0)
	{
		--CheatHKeys::smoothValue;
		return true;
	}
	else if (GetAsyncKeyState(VK_F4))
	{
		++CheatHKeys::smoothValue;
		return true;
	}
	else if (GetAsyncKeyState(VK_F5) & 1 &&
		CheatHKeys::fovValue > 10)
	{
		CheatHKeys::fovValue -= 10;
		return true;
	}
	else if (GetAsyncKeyState(VK_F6) & 1 &&
		CheatHKeys::fovValue < 100)
	{
		CheatHKeys::fovValue += 10;
		return true;
	}
	else if (GetAsyncKeyState(VK_F8) & 1)
	{
		CheatHKeys::bESP = !CheatHKeys::bESP;
		return true;
	}
	else if (GetAsyncKeyState(VK_RBUTTON) & 1)
	{
		CheatHKeys::bAimbot = !(CheatHKeys::bAimbot);
		return true;
	}
	else if (GetAsyncKeyState(VK_F9) & 1)
	{
		CheatHKeys::bTeamCheck = !CheatHKeys::bTeamCheck;
		return true;
	}

	return false;
}

bool CheatHKeys::SetTeamCheckDefaultValue(bool pIsDeathMatch)
{
	if (pIsDeathMatch)
	{
		CheatHKeys::bTeamCheck = false;
		bInitTeamCheck = true;
		return true;
	}

	return false;
}