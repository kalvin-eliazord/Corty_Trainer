#include "AimbotOptions.h"

bool AimbotOptions::IsOptionChanged()
{
	if (GetAsyncKeyState(VK_F4) & 1)
	{
		++AimbotOptions::smoothValue;
		return true;
	}
	else if (GetAsyncKeyState(VK_F3) & 1 && AimbotOptions::smoothValue > 0)
	{
		--AimbotOptions::smoothValue;
		return true;
	}
	else if (GetAsyncKeyState(VK_F2) & 1)
	{
		AimbotOptions::bTargetLock = !AimbotOptions::bTargetLock;
		return true;
	}
	else if (GetAsyncKeyState(VK_F5) & 1 && AimbotOptions::fovValue > 10)
	{
		AimbotOptions::fovValue -= 10;
		return true;
	}
	else if (GetAsyncKeyState(VK_F6) & 1)
	{
		AimbotOptions::fovValue += 10;
		return true;

	}
	else if (GetAsyncKeyState(VK_F9) & 1)
	{
		AimbotOptions::bAimbot = !(AimbotOptions::bAimbot);
		return true;
	}

	return false;
}