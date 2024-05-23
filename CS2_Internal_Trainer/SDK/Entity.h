#pragma once
#include "BoneJoint.h"
#include "MyOffsets.h"
#include "MyPointers.h"
#include "Vector3.h"
#include <string>
#include <bitset>

struct Controller
{
	int32_t hPawn{};
	std::string sEntName{};
};

struct Pawn
{
	bool bDormant{};
	int32_t iTeamNum{};
	int32_t iHealth{};
	Vector3 headBonePos{};
	Vector3 pelvisBonePos{};
	Vector3 vLastCameraPos{};
	Vector3 vAngEyeAngle{};
	std::bitset<64> bSpottedMask{};
};

class Entity
{
private:
	bool isEntInit{ false };
	Pawn pawnBase{};
	Controller cBase{};

	// Update Controller data
	bool SetHPawn(intptr_t pCBaseAddr);
	bool SetPawnBase(intptr_t pCBaseAddr);
	bool SetEntName(intptr_t pCBaseAddr);
	bool UpdateController(intptr_t pCBaseAddr);

	// Update Pawn Data
	bool SetHealth(intptr_t pPawnAddr);
	bool SetHeadPos(intptr_t pPawnAddr);
	bool SetPelvisPos(intptr_t pPawnAddr);
	bool SetSpottedMask(intptr_t pPawnAddr);
	bool SetIsDormant(intptr_t pPawnAddr);
	bool SetTeamNum(intptr_t pPawnAddr);
	bool SetvAngEyeAngle(intptr_t pPawnAddr);
	bool SetvLastCameraPos(intptr_t pPawnAddr);
	intptr_t GetBoneArrayBase(intptr_t pPawnAddr);
	bool UpdatePawn(intptr_t pPawnAddr);

public:
	Pawn GetPawnBase();
	Controller GetCBase();

	bool IsEntInit();
	Entity(intptr_t pCBaseAddr);
	Entity();
};