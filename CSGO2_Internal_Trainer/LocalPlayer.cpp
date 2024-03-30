#include "LocalPlayer.h"

Entity* LocalPlayer::Get()
{
    Entity* localPlayerBaseAddr{ *(Entity**)GamePointer::localPlayerPtr };
    return localPlayerBaseAddr;
}

void LocalPlayer::SetViewAngle(Vector3& targetAngle, const int pSmoothValue)
{
    if (pSmoothValue != 0)
    {
        TargetManager::SetAngleSmoothing(targetAngle, pSmoothValue);
    }
    else
    {
        float* lpPitch{ (float*)(GamePointer::lp_Pitch_Input) };
        float* lpYaw{ (float*)(GamePointer::lp_Yaw_Input) };

        *lpPitch = targetAngle.x;
        *lpYaw = targetAngle.y;
    }
}