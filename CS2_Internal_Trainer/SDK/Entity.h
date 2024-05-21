#pragma once
#include "BoneJoint.h"
#include "GamePointers.h"
#include "MyOffset.h"
#include "Vector3.h"
#include <string>
#include <bitset>
// cs2 dumper
#include "offsets.hpp"
#include "client.dll.hpp"

struct Controller
{
	int32_t hPawn{ NULL };
	std::string sEntName{};
};

struct Pawn
{
	bool bDormant{ NULL };
	int32_t iTeamNum{ NULL };
	int32_t iHealth{ NULL };
	Vector3 headBonePos{};
	Vector3 pelvisBonePos{};
	Vector3 vLastCameraPos{};
	Vector3 vAngEyeAngle{};
	std::bitset<64> bSpottedMask{ NULL };
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
	bool SetSpottedId(intptr_t pPawnAddr);
	bool SetIsDormant(intptr_t pPawnAddr);
	bool SetTeamNum(intptr_t pPawnAddr);
	bool SetvAngEyeAngle(intptr_t pPawnAddr);
	bool SetvLastCameraPos(intptr_t pPawnAddr);
	BoneJoint* GetBoneArrayBase(intptr_t pPawnAddr);
	bool UpdatePawn(intptr_t pPawnAddr);

public:
	Pawn GetPawnBase();
	Controller GetCBase();

	bool IsEntInit();
	Entity(intptr_t pCBaseAddr);
	Entity();
};