#include "LocalPlayer.h"

Entity LocalPlayer::GetEntity()
{
	return Entity (*reinterpret_cast<Controller**>(GamePointer::localPlayerContPtr));
}

Controller* LocalPlayer::GetController()
{
	return reinterpret_cast<Controller*>(GamePointer::localPlayerContPtr);
}

Pawn* LocalPlayer::GetPawn()
{
	return GetEntity().GetPawnBase();
}

float* LocalPlayer::GetPitchPtr()
{
	return static_cast<float*>(GamePointer::lp_Pitch_Input);
}

float* LocalPlayer::GetYawPtr()
{
	return static_cast<float*>(GamePointer::lp_Yaw_Input);
}

void LocalPlayer::SetViewAngle(Vector3& targetAngle)
{
	float* lpPitch{ GetPitchPtr() };
	float* lpYaw{ GetYawPtr() };

	*lpPitch = targetAngle.x;
	*lpYaw = targetAngle.y;
}