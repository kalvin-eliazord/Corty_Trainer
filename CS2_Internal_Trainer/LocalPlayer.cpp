#include "LocalPlayer.h"

intptr_t* LocalPlayer::Get()
{
	return reinterpret_cast<intptr_t*>(GamePointer::localPlayerPtr);
}

float* LocalPlayer::GetPitchPtr()
{
	return reinterpret_cast<float*>(GamePointer::lp_Pitch_Input);
}

float* LocalPlayer::GetYawPtr()
{
	return reinterpret_cast<float*>(GamePointer::lp_Yaw_Input);
}

void LocalPlayer::SetViewAngle(Vector3& targetAngle)
{
	float* lpPitch{ GetPitchPtr() };
	float* lpYaw{ GetYawPtr() };

	*lpPitch = targetAngle.x;
	*lpYaw = targetAngle.y;
}