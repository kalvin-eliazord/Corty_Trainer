#pragma once
#include "MyOffsets.h"
#include "MyPointers.h"
#include "Vector3.h"
#include <string>
#include <bitset>

enum Bone;
struct BoneJoint;

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
	Vector3 vLastCameraPos{};
	Vector3 vAngEyeAngle{};
	std::bitset<64> bSpottedMask{};
};

class Entity
{
private:
	bool isEntInit{ false };
	intptr_t pawnBaseAddr{0};
	intptr_t cBaseAddr{0};
	int iIndex{};

	Pawn pawnBase{};
	Controller cBase{};

	// Init Controller members
	bool SetHPawn();
	bool SetPawnBase();
	bool SetEntName();
	bool UpdateController();

	// Init Pawn members
	bool SetHealth();
	bool SetSpottedMask();
	bool SetIsDormant();
	bool SetTeamNum();
	bool SetvAngEyeAngle();
	bool SetvLastCameraPos();
	intptr_t GetBoneArrayBase();
	bool UpdatePawn();

public:
	int GetIndex();
	Pawn GetPawnBase();
	Controller GetCBase();
	Vector3 GetBonePos(Bone pBone);

	bool IsEntInit();
	Entity(intptr_t pCBaseAddr, int pIndex);
	Entity();
};

struct BoneJoint
{
	Vector3 pos{};
	float scale;
	float rotation[4];
};

enum Bone
{
	head = 6,
	neck_0 = 5,
	spine_1 = 4,
	spine_2 = 2,
	pelvis = 0,
	arm_upper_L = 8,
	arm_lower_L = 9,
	hand_L = 10,
	arm_upper_R = 13,
	arm_lower_R = 14,
	hand_R = 15,
	leg_upper_L = 22,
	leg_lower_L = 23,
	ankle_L = 24,
	leg_upper_R = 25,
	leg_lower_R = 26,
	ankle_R = 27,
};