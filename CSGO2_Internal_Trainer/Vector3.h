#pragma once
#include "header.h"

struct Vector3
{
	float x, y, z;
    Vector3 operator + (const Vector3& pVect3) const { return Vector3(x + pVect3.x, y + pVect3.y, z + pVect3.z); }
    Vector3 operator - (const Vector3& pVect3) const { return Vector3(x - pVect3.x, y - pVect3.y, z - pVect3.z); }
    Vector3 operator * (const float& pValue) const { return Vector3(x * pValue, y * pValue, z * pValue); }
    Vector3 operator / (const float& pValue) const { return Vector3(x / pValue, y / pValue, z / pValue); }
    Vector3& operator += (const Vector3& pVect3) { return *this = *this + pVect3; }
    Vector3& operator -= (const Vector3& pVect3) { return *this = *this - pVect3; }
    Vector3& operator *= (const float& pValue) { return *this = *this * pValue; }
    Vector3& operator /= (const float& pValue) { return *this = *this / pValue; }
};