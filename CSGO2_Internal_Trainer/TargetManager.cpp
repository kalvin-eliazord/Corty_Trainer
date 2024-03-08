#include "TargetManager.h"

Entity* TargetManager::GetNearestTarget(Entity* pLocalPlayer, std::vector<Entity*> pTargetList)
{
    float oldCoefDist{ FLT_MAX };
    Entity* nearestTarget{ nullptr };

    for (auto currTarget : pTargetList)
    {
        // crossHair Distance
        const Vector3 currTargetAngle{ GetTargetAngle(pLocalPlayer, currTarget) };
        const float newAngleDist{ BasicMath::GetMagnitude(pLocalPlayer->angles, currTargetAngle)};

        // body position distance
        const float newBodyPosDist{ BasicMath::GetMagnitude(pLocalPlayer->body_pos, currTarget->body_pos) };

        // Based on crosshair + body position distance
        const float newCoefDist{ (newAngleDist * 0.9f) + (newBodyPosDist * 0.1f) };

        if (newCoefDist < oldCoefDist)
        {
            oldCoefDist   = newCoefDist;
            nearestTarget = currTarget;
        }
    }

    return nearestTarget;
}

float TargetManager::GetClamp(const float p_fValueToClamp, const float p_fMin, const float p_fMax)
{
    const float fValueClamped{ p_fValueToClamp < p_fMin ? p_fMin : p_fValueToClamp };
    return fValueClamped > p_fMax ? p_fMax : fValueClamped;
}

Vector3 TargetManager::GetTargetAngle(Entity* pLocalPlayer, Entity* target)
{

    const Vector3 delta{ BasicMath::GetDelta(pLocalPlayer->body_pos, target->body_pos) };
    
    const float magnitude{ BasicMath::GetMagnitude(delta) };

    Vector3 targetAngle{};

    //old w accroupie bug
    targetAngle.x = atanf(delta.z / magnitude) * 57.2957795f;
    targetAngle.y = atanf(delta.y / delta.x) * 57.2957795f;

    if (delta.x >= 0.0f)
        targetAngle.y += 180.0f;

    targetAngle.x = GetClamp(targetAngle.x, -89, 89);

    return targetAngle;
}

void TargetManager::SetAngleSmoothing(Vector3& lpAngle, Vector3& pTargetAngle, const int pSmoothValue)
{
    float* lpPitch{ (float*)(GameOffset::Client::lp_Pitch_Input) };
    float* lpYaw{ (float*)(GameOffset::Client::lp_Yaw_Input) };

    Vector3 deltaAngle{ BasicMath::GetDelta(pTargetAngle, lpAngle) };

    if (lpAngle.x != pTargetAngle.x)
        *lpPitch += deltaAngle.x / pSmoothValue;

    if (lpAngle.y != pTargetAngle.y)
        *lpYaw   += deltaAngle.y / pSmoothValue;
}