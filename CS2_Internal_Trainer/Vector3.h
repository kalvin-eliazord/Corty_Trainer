#pragma once

struct Vector3
{
	float x, y, z;

    Vector3 operator+(const Vector3& d);
    Vector3 operator-(const Vector3& d);
    Vector3 operator*(const Vector3& d);
    Vector3 operator*(const float& d);
};