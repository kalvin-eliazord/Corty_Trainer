#include "LocalPlayer.h"

Entity LocalPlayer::GetEntity()
{
	static Entity localPlayer(*reinterpret_cast<Controller**>(GamePointer::localPlayerContPtr));

	return localPlayer;
}

Controller* LocalPlayer::GetController()
{
	return GetEntity().GetControllerBase();
}

Pawn* LocalPlayer::GetPawn()
{
	return GetEntity().GetPawnBase();
}

float* LocalPlayer::GetPitchPtr()
{
	static float* pitchPtr { static_cast<float*>(GamePointer::lp_Pitch_Input) };

	return pitchPtr;
}

float* LocalPlayer::GetYawPtr()
{
	static float* yawPtr { static_cast<float*>(GamePointer::lp_Yaw_Input) };

	return yawPtr;
}

void LocalPlayer::SetViewAngle(const Vector3& targetAngle)
{
	*GetPitchPtr() = targetAngle.x;
	*GetYawPtr() = targetAngle.y;
}