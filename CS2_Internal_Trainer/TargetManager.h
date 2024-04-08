#pragma once
#include <vector>
#include "LocalPlayer.h"
#include "Entity.h"

namespace TargetManager
{
	bool IsGoodTarget(Pawn* pLocalPlayer, Entity* entityPtr, char* pGameTypeId);
	std::vector<Pawn*> GetTargetList(Pawn* pLocalPlayer, char* pGameTypeId);

	float GetMagnitude(Vector3 pVec);
	Pawn* GetNearestTarget(Pawn* pLocalPlayer, std::vector<Pawn*> pTargetList);
	Vector3 GetTargetAngle(Pawn* pLocalPlayer, Pawn* target);

	void SetViewAngleSmooth(Vector3& pTargetAngle, const int pSmoothValue);
	float NormalizePitch(const float pPitch);
	float NormalizeYaw(float pYaw);
};