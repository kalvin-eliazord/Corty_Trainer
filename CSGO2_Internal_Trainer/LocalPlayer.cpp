#include "LocalPlayer.h"

Entity* LocalPlayer::Get()
{
    Entity* localPlayerBaseAddr{ *(Entity**)(GameOffset::Client::localPlayer) };
    return localPlayerBaseAddr;
}

void LocalPlayer::SetViewAngle(Vector3& lpAngle, Vector3& targetAngle, const int pSmoothValue)
{
    if (pSmoothValue != 0)
    {
        TargetManager::SetAngleSmoothing(lpAngle, targetAngle, pSmoothValue);
    }
    else
    {
        float* lpPitch{ (float*)(GameOffset::Client::lp_Pitch_Input) };
        float* lpYaw{ (float*)(GameOffset::Client::lp_Yaw_Input) };

        *lpPitch = targetAngle.x;
        *lpYaw = targetAngle.y;
    }
}