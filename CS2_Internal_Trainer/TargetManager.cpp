#include "TargetManager.h"

float TargetManager::NormalizePitch(const float pPitch)
{
    if (pPitch < -89) return -89;

    if (pPitch > 89) return 89;

    return pPitch;
}

float TargetManager::NormalizeYaw(float pYaw)
{
    while (pYaw > 180) pYaw -= 360;

    while (pYaw < -180) pYaw += 360;

    return pYaw;
}

float TargetManager::GetMagnitude(Vector3 pVec)
{
    return ::sqrtf((pVec.x * pVec.x) +
                   (pVec.y * pVec.y) + 
                   (pVec.z * pVec.z));
}

Entity* TargetManager::GetNearestTarget(Entity* pLocalPlayer, std::vector<Entity*> pTargetList)
{
    float oldCoef{ FLT_MAX };
    Entity* nearestTarget{ nullptr };

    for (auto currTarget : pTargetList)
    {
        const Vector3 currTargetAngle{ GetTargetAngle(pLocalPlayer, currTarget) };

        Vector3 deltaAngle{ pLocalPlayer->angles - currTargetAngle };
        const float currAngleDist{ GetMagnitude(deltaAngle) };

        Vector3 deltaPosition{ pLocalPlayer->body_pos - currTarget->body_pos };
        const float currBodyPosDist{ GetMagnitude(deltaPosition) };

        const float currCoef{ currAngleDist * 0.8f + currBodyPosDist * 0.2f};

        if (oldCoef > currCoef)
        {
            oldCoef = currCoef;
            nearestTarget = currTarget;
        }
    }

    return nearestTarget;
}

Vector3 TargetManager::GetTargetAngle(Entity* pLocalPlayer, Entity* target)
{
    const Vector3 deltaPos{ pLocalPlayer->body_pos - target->body_pos };
    
    const float magnitudePos{ GetMagnitude(deltaPos) };

    Vector3 targetAngle{};
    constexpr float radToDegree{ 57.2957795f };

    targetAngle.x = atanf(deltaPos.z / magnitudePos) * radToDegree;
    targetAngle.y = atanf(deltaPos.y / deltaPos.x) * radToDegree;

    if (deltaPos.x >= 0.0f)
        targetAngle.y += 180.0f;

    targetAngle.x = NormalizePitch(targetAngle.x);

    return targetAngle;
}

void TargetManager::SetViewAngleSmooth(Vector3& pTargetAngle, const int pSmoothValue)
{
    float* lp_Pitch{ LocalPlayer::GetPitchPtr()};
    float* lp_Yaw{ LocalPlayer::GetYawPtr() };

    Vector3 lp_Angle{ *lp_Pitch, *lp_Yaw, 0 };

    Vector3 deltaAngle{ pTargetAngle - lp_Angle };

    deltaAngle.y = NormalizeYaw(deltaAngle.y);

    if (*lp_Pitch != pTargetAngle.x)
        *lp_Pitch += deltaAngle.x / pSmoothValue;

    if (*lp_Yaw != pTargetAngle.y)
        *lp_Yaw += deltaAngle.y / pSmoothValue;
}