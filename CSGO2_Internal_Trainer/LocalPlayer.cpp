#include "LocalPlayer.h"

intptr_t LocalPlayer::Get()
{
    return *(intptr_t*)(GameOffset::Client::localPlayer);
}

void LocalPlayer::SetViewAngle(const Vector3& targetAngle)
{
    float* lpPitch{ (float*)(GameOffset::Client::lp_Pitch_Input) };
    float* lpYaw{ (float*)(GameOffset::Client::lp_Yaw_Input) };

    *lpPitch = targetAngle.x;
    *lpYaw   = targetAngle.y;
}