#include "LocalPlayer.h"

Entity* LocalPlayer::Get()
{
    Entity* localPlayerBaseAddr{ nullptr };
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
        float* lpPitch{ (float*)(GameOffset::Client::lp_Pitch_Input) };
        float* lpYaw{ (float*)(GameOffset::Client::lp_Yaw_Input) };

        *lpPitch = targetAngle.x;
        *lpYaw = targetAngle.y;
    }
}