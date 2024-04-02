#pragma once
#include "header.h"

struct Vector3
{
	float x, y, z;

    Vector3 operator+(Vector3 d);
    Vector3 operator-(Vector3 d);
    Vector3 operator*(Vector3 d);
    Vector3 operator*(float d);
};