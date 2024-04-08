#include "TargetManager.h"

bool TargetManager::IsGoodTarget(Pawn* pLocalPlayer, Entity* entityPtr, char* pGameTypeId)
{
    // iteration empty
    if (!entityPtr or !*(intptr_t*)entityPtr or !(intptr_t)entityPtr)
        return false;

    // Entity dead
    if (entityPtr->pawnBase->health < 1)
        return false;

    constexpr char deathmatchId{ 0x27 };

    // don't check for team if there is no team
    if (*pGameTypeId != deathmatchId)
    {
        // Same team as LP
        if (pLocalPlayer->team_variable == entityPtr->pawnBase->team_variable)
            return false;
    }

    // If Entity is behind the wall
    //bSpottedMask

    return true;
}

std::vector<Pawn*> TargetManager::GetTargetList(Pawn* pLocalPlayer, char* pGameTypeId)
{
    std::vector<Pawn*> targetsPawn{};
    intptr_t* entityListBase{ GamePointer::entityListBasePtr };

    for (int i{ 1 }; i < 64; ++i)
    {
        Entity currEntity(reinterpret_cast<intptr_t*>(reinterpret_cast<intptr_t>(entityListBase) + i * 0x78));

        if (!currEntity.isPawnInit) continue;

        if (!IsGoodTarget(pLocalPlayer, &currEntity, pGameTypeId))
            continue;

        targetsPawn.push_back(currEntity.pawnBase);
    }

    return targetsPawn;
}

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

Vector3 TargetManager::GetTargetAngle(Pawn* pLocalPlayer, Pawn* target)
{
    Vector3 targetAngle{NULL};

    if (target == nullptr) return targetAngle;

    const Vector3 deltaPos{ pLocalPlayer->body_pos - target->body_pos };

    const float magnitudePos{ GetMagnitude(deltaPos) };

    constexpr float radToDegree{ 57.2957795f };

    targetAngle.x = atanf(deltaPos.z / magnitudePos) * radToDegree;
    targetAngle.y = atanf(deltaPos.y / deltaPos.x) * radToDegree;

    if (deltaPos.x >= 0.0f)
        targetAngle.y += 180.0f;

    targetAngle.x = NormalizePitch(targetAngle.x);

    return targetAngle;
}

Pawn* TargetManager::GetNearestTarget(Pawn* pLocalPlayer, std::vector<Pawn*> pTargetList)
{
    float oldCoef{ FLT_MAX };
    Pawn* nearestTarget{ nullptr };

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