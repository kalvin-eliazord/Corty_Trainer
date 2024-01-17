#include "Aimbot.h"

int Aimbot::GetNearestTarget(Entity* currEntity, int currEntityIndex)
{
    Entity* localPlayer{Entity::GetLocalPlayer()};

    float actualNearestTarget = {
        (localPlayer->body_pos.x - currEntity->body_pos.x) +
        (localPlayer->body_pos.y - currEntity->body_pos.y) +
        (localPlayer->body_pos.z - currEntity->body_pos.z)
    };

    // if the current entity is nearest then the old one, return the current entity index
    if (actualNearestTarget < this->oldNearestTarget)
    {
        this->oldNearestTarget = actualNearestTarget;
        return currEntityIndex;
    }

    return this->oldEntityIndex;
}

Vector3 Aimbot::GetTargetAngle(Entity* target)
{
    Entity* localPlayer{ Entity::GetLocalPlayer() };

    const Vector3 delta{
         (localPlayer->body_pos.x - target->body_pos.x) +
        (localPlayer->body_pos.y - target->body_pos.y) +
        (localPlayer->body_pos.z - target->body_pos.z)};

    const float hypotenuse{
        (delta.x * delta.x) +
        (delta.y * delta.y) +
        (delta.z * delta.z) };

    Vector3 targetAngle{};

    targetAngle.x = 0;
    targetAngle.y = 0;

    return targetAngle;
}
