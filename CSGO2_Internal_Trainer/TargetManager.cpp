#include "TargetManager.h"

float TargetManager::GetClamp(const float p_fValueToClamp, const float p_fMin, const float p_fMax)
{
    const float fValueClamped{ p_fValueToClamp < p_fMin ? p_fMin : p_fValueToClamp };
    return fValueClamped > p_fMax ? p_fMax : fValueClamped;
}

void TargetManager::SetNearestTarget(Entity* pTarget, int currEntityIndex)
{
    Entity* localPlayer{LocalPlayer::Get()};

    Vector3 currTargetAngle{ GetTargetAngle(pTarget) };

    Vector3 currAngleDist{
        (abs(localPlayer->angles.x - currTargetAngle.x)),
        (abs(localPlayer->angles.y - currTargetAngle.y)),
        (0) };

    const float lpAngle{ abs(localPlayer->angles.x + localPlayer->angles.y) };
    const float targetAngle{ currTargetAngle.x + currTargetAngle.y };

    // to fix
    if (currAngleDist < this->oldAngleDist)
    {
        this->oldAngleDist = currAngleDist;
        this->oldEntityIndex = currEntityIndex;
    }
}

int TargetManager::GetNearestTargetIndex()
{
    return this->oldEntityIndex;
}

Vector3 TargetManager::GetTargetAngle(Entity* target)
{
    Entity* localPlayer{ LocalPlayer::Get() };

    const Vector3 delta{
         (localPlayer->body_pos.x - target->body_pos.x),
        (localPlayer->body_pos.y - target->body_pos.y),
        (localPlayer->body_pos.z - target->body_pos.z)};

    const float hypotenuse{sqrt(
        (delta.x * delta.x) +
        (delta.y * delta.y) +
        (delta.z * delta.z)) };

    Vector3 targetAngle{};

    targetAngle.x = atanf(delta.z / hypotenuse) * 57.2957795f;
    targetAngle.y = atanf(delta.y / delta.x) * 57.2957795f;

    if (delta.x >= 0.0f)
        targetAngle.y += 180.0f;

    targetAngle.x = GetClamp(targetAngle.x, -89, 89);

    return targetAngle;
}