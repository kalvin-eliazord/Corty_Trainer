#include "TargetManager.h"

Entity* TargetManager::GetNearestTarget(Entity* pLocalPlayer, std::vector<Entity*> pTargetList)
{
    float oldAngleDist{ FLT_MAX };
    Entity* nearestTarget{ nullptr };

    for (auto currTarget : pTargetList)
    {
        // crossHair Distance
        const Vector3 currTargetAngle{ GetTargetAngle(pLocalPlayer, currTarget) };
        const float newAngleDist{ BasicMath::GetMagnitude(pLocalPlayer->angles, currTargetAngle)};

        // body position distance
       // const float newBodyPosDist{ BasicMath::GetMagnitude(pLocalPlayer->body_pos, currTarget->body_pos) };

        if (newAngleDist < oldAngleDist)
        {
            oldAngleDist = newAngleDist;
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

    targetAngle.x = atanf(delta.z / magnitude) * 57.2957795f;
    targetAngle.y = atanf(delta.y / delta.x) * 57.2957795f;

    if (delta.x >= 0.0f)
        targetAngle.y += 180.0f;

    targetAngle.x = GetClamp(targetAngle.x, -89, 89);

    return targetAngle;
}

void TargetManager::SetAngleSmoothing(Vector3& pTargetAngle, const int pSmoothValue)
{
    float* lp_Pitch{ (float*)(GameOffset::Client::lp_Pitch_Input) };
    float* lp_Yaw{ (float*)(GameOffset::Client::lp_Yaw_Input) };

    Vector3 lp_Angle{ *lp_Pitch, *lp_Yaw, 0 };

    Vector3 deltaAngle{ BasicMath::GetDelta(pTargetAngle, lp_Angle) };

    if (deltaAngle.y > 180)
    {
        deltaAngle.y -= 360;
    }
    if (deltaAngle.y < -180)
    {
        deltaAngle.y += 360;
    }

    //system("cls");
   // std::cout << "DeltaAngle X: " << deltaAngle.x << " Y:" << deltaAngle.y << "\t \n";
    //std::cout << "pTargetAngle X: " << pTargetAngle.x << " Y:" << pTargetAngle.y << "\t \n";

    if (*lp_Pitch != pTargetAngle.x)
        *lp_Pitch += deltaAngle.x / pSmoothValue;

    if (*lp_Yaw != pTargetAngle.y)
        *lp_Yaw += deltaAngle.y / pSmoothValue;
}