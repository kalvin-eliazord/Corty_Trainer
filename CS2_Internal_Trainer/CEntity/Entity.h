#pragma once
#include "BoneJoint.h"
#include "Pointer.h"
#include "Offset.h"
#include "Controller.h"
#include "Pawn.h"
#include "Vector3.h"
#include <string>
#include <bitset>

class Entity
{
private:
	Controller* cBase{nullptr};
	Pawn* pawnBase{nullptr};
	bool IsPawnInit{ false };

	void SetPawnBase();
public:
	Pawn* GetPawnBase();
	bool GetIsPawnInit();
	Vector3 GetHeadPos();
	Vector3 GetPelvisPos();
	Entity(Controller* pSrc);
	intptr_t* GetBoneArrayBase();
	std::bitset<64> GetSpottedId();
	Controller* GetControllerBase();
};