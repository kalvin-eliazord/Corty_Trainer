#include "Vector3.h"

Vector3 Vector3::operator+(const Vector3& d)
{
    return { x + d.x, y + d.y, z + d.z };
}

Vector3 Vector3::operator-(const Vector3& d)
{
    return { x - d.x, y - d.y, z - d.z };
}

Vector3 Vector3::operator*(const Vector3& d)
{
    return { x * d.x, y * d.y, z * d.z };
}

Vector3 Vector3::operator*(const float& d)
{
    return { x * d, y * d, z * d };
}