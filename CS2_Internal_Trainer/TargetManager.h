#pragma once
#include <vector>
#include "LocalPlayer.h"
#include "Entity.h"

namespace TargetManager
{
	std::vector<Pawn*> GetTargetsPawn(Pawn* pLocalPlayerPawn, int_least8_t* pGameTypeId);
	bool IsGoodTarget(Pawn* pLocalPlayerPawn, Entity* entityPtr, int_least8_t* pGameTypeId);

	Pawn* GetNearestTarget(Pawn* pLocalPlayerPawn, std::vector<Pawn*> pTargetsPawn);
	Vector3 GetTargetAngle(Vector3& pLocalPlayerPawn, Vector3& pTargetsPawn);
	float GetMagnitude(const Vector3& pVec);

	void SetViewAngleSmooth(Vector3& pTargetAngle, const int pSmoothValue);
	float NormalizePitch(const float pPitch);
	float NormalizeYaw(float pYaw);
};