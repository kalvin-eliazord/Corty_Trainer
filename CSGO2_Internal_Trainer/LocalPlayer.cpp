#include "LocalPlayer.h"

Entity* LocalPlayer::Get()
{
	Entity* localPlayerBaseAddr{ *(Entity**)GamePointer::localPlayerPtr };
	return localPlayerBaseAddr;
}

float* LocalPlayer::GetPitchPtr()
{
	return (float*)(GamePointer::lp_Pitch_Input);
}

float* LocalPlayer::GetYawPtr()
{
	return (float*)(GamePointer::lp_Yaw_Input);
}

void LocalPlayer::SetViewAngle(Vector3& targetAngle)
{
	float* lpPitch{ GetPitchPtr() };
	float* lpYaw{ GetYawPtr() };

	*lpPitch = targetAngle.x;
	*lpYaw = targetAngle.y;
}