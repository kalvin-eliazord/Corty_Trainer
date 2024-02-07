#include "Aimbot.h"

float Aimbot::GetClamp(const float p_fValueToClamp, const float p_fMin, const float p_fMax)
{
    const float fValueClamped{ p_fValueToClamp < p_fMin ? p_fMin : p_fValueToClamp };
    return fValueClamped > p_fMax ? p_fMax : fValueClamped;
}

void Aimbot::SetNearestTarget(Entity* currEntity, int currEntityIndex)
{
    Entity* localPlayer{LocalPlayer::Get()};

    Vector3 delta = {
        (localPlayer->body_pos.x - currEntity->body_pos.x) +
        (localPlayer->body_pos.y - currEntity->body_pos.y) +
        (localPlayer->body_pos.z - currEntity->body_pos.z)
    };

    const float currDistance{ sqrt((delta.x * delta.x) + (delta.y * delta.y) + (delta.z * delta.z)) };

    // if the current entity is nearest then the old one, return the current entity index
    if (currDistance < this->oldDistance)
    {
        this->oldDistance = currDistance;
        this->oldEntityIndex = currEntityIndex;
    }
}

int Aimbot::GetNearestTargetIndex()
{
    return this->oldEntityIndex;
}

Vector3 Aimbot::GetTargetAngle(Entity* target)
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


    /*


 
*/

    targetAngle.x = asinf(delta.z / hypotenuse) * -57.2957795f;
    targetAngle.y = atanf(delta.y / delta.x) * 57.2957795f;

    while (targetAngle.y < -180.0f)
        targetAngle.y += 360.0f;

    while (targetAngle.y > 180.0f)
        targetAngle.y -= 360.0f;

    if (delta.x >= 0.0f)
        targetAngle.y += 180.0f;

    targetAngle.x = GetClamp(targetAngle.x, -89, 89);

    return targetAngle;
}
