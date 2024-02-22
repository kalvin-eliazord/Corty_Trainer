#include "LocalPlayer.h"

Entity* LocalPlayer::Get()
{
    return *(Entity**)(GameOffset::Client::localPlayer);
}

void LocalPlayer::SetViewAngle(const float pPitch, const float pYaw)
{
    float* lpPitch{ (float*)(GameOffset::Client::lp_Pitch_Input) };
    float* lpYaw{ (float*)(GameOffset::Client::lp_Yaw_Input) };

    *lpPitch = pPitch;
    *lpYaw   = pYaw;
}