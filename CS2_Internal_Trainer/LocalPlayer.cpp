#include "LocalPlayer.h"

Controller* LocalPlayer::GetController()
{
	return reinterpret_cast<Controller*>(GamePointer::localPlayerContPtr);
}

Pawn* LocalPlayer::GetPawn()
{
	return reinterpret_cast<Pawn*>(GamePointer::localPlayerPawnPtr);
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