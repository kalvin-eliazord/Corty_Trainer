#include "LocalPlayer.h"

Entity LocalPlayer::GetEntity()
{
	Entity lpEntityPtr(GamePointers::GetLpControllerPtr());
	return lpEntityPtr;
}

Controller LocalPlayer::GetController()
{
	return GetEntity().GetCBase();
}

Pawn LocalPlayer::GetPawn()
{
	return GetEntity().GetPawnBase();
}

Vector3* LocalPlayer::GetViewAnglesPtr()
{
	Vector3* viewAngles{ GamePointers::GetViewAnglesPtr() };
	return viewAngles;
}

bool LocalPlayer::SetViewAngles(const Vector3& targetAngle)
{
	Vector3* viewAnglesPtr{ GamePointers::GetViewAnglesPtr() };

	if (!viewAnglesPtr) return false;

	viewAnglesPtr->x = targetAngle.x;
	viewAnglesPtr->y = targetAngle.y;

	return true;
}

bool LocalPlayer::SetSmoothViewAngles(Vector3 pTargetAngle, const int pSmoothValue)
{
	Vector3* lpViewAngles{ GamePointers::GetViewAnglesPtr() };

	Vector3 deltaAngle{  pTargetAngle - *lpViewAngles };

	Aimbot::NormalizeYaw(deltaAngle.y);

	if (lpViewAngles->x != pTargetAngle.x)
		lpViewAngles->x += deltaAngle.x / pSmoothValue;

	if (lpViewAngles->y != pTargetAngle.y)
		lpViewAngles->y += deltaAngle.y / pSmoothValue;

	return true;
}
