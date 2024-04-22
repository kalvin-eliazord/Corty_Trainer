#include "LocalPlayer.h"

Entity LocalPlayer::GetEntity()
{
	static Entity localPlayer(*reinterpret_cast<Controller**>(Pointer::lp_Controller));

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
	static float* pitchPtr { static_cast<float*>(Pointer::lp_Pitch) };

	return pitchPtr;
}

float* LocalPlayer::GetYawPtr()
{
	static float* yawPtr { static_cast<float*>(Pointer::lp_Yaw) };

	return yawPtr;
}

void LocalPlayer::SetViewAngle(const Vector3& targetAngle)
{
	*GetPitchPtr() = targetAngle.x;
	*GetYawPtr() = targetAngle.y;
}