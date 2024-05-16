#pragma once
#include "BoneJoint.h"
#include "Pointer.h"
#include "Offset.h"
#include "Vector3.h"
#include <string>
#include <bitset>

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

struct Controller;
struct Pawn;

class Entity
{
private:
	Controller* cBase{nullptr};
	Pawn* pawnBase{nullptr};
	bool IsPawnInit{ false };

	void SetPawnBase();
public:
	bool IsDormant();
	Pawn* GetPawnBase();
	bool GetIsPawnInit();
	Vector3 GetHeadPos();
	Vector3 GetPelvisPos();
	Entity(Controller* pSrc);
	intptr_t* GetBoneArrayBase();
	std::bitset<64> GetSpottedId();
	Controller* GetControllerBase();
};

//TODO REPLACE BY PATTERN SCANNING USING CS2 DUMPER?

struct Controller
{
	union
	{
		DEFINE_MEMBER_N(char, entName, 0x638);
		DEFINE_MEMBER_N(int32_t, pawnOffset, 0x7e4);
	};
};

struct Pawn
{
	union
	{
		//              Type     Name    offset
		DEFINE_MEMBER_N(int, health, 0x334);
		DEFINE_MEMBER_N(int8_t, team_variable, 0x3CB);
		DEFINE_MEMBER_N(Vector3, lastClipCameraPos, 0x12DC);
		DEFINE_MEMBER_N(Vector3, angles, 0x1390);
	};
};