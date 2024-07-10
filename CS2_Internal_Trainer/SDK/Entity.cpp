#include "Entity.h"

Entity::Entity(intptr_t pCBaseAddr, int pIndex)
	: cBaseAddr{ pCBaseAddr }
	, iIndex{ pIndex }
{
	if (cBaseAddr && UpdateController())
		isEntInit = true;
}

Entity::Entity()
{}

bool Entity::UpdateController()
{
	if (!SetEntName()) return false;
	if (!SetHPawn()) return false;
	if (!SetPawnBase()) return false;

	return true;
}

bool Entity::UpdatePawn()
{
	if (!SetIsDormant()) return false;
	if (!SetHealth()) return false;
	if (!SetSpottedMask()) return false;
	if (!SetTeamNum()) return false;
	if (!SetvAngEyeAngle()) return false;
	if (!SetvLastCameraPos()) return false;

	return true;
}

bool Entity::SetHPawn()
{
	cBase.hPawn = MyPointers::ReadInternalMem<int32_t>(cBaseAddr, { MyOffset::Controller::hPawn });

	return cBase.hPawn ? true : false;
}

bool Entity::SetPawnBase()
{
	const intptr_t cGameEntity{ MyPointers::GetGameEntitySystemBase() + MyOffset::EntityListPtr };
	const int32_t pawnListOffset{ 8 * ((cBase.hPawn & 0x7FFF) >> 9) };
	const int32_t pawnBaseOffset{ 0x78 * (cBase.hPawn & 0x1FF) };

	intptr_t pawnBase{};

	if (pawnListOffset)
		pawnBase = static_cast<intptr_t>(MyPointers::ReadInternalMem<uintptr_t>(cGameEntity, { pawnListOffset, pawnBaseOffset }));
	else
		pawnBase = static_cast<intptr_t>(MyPointers::ReadInternalMem<uintptr_t>(*reinterpret_cast<intptr_t*>(cGameEntity), { pawnBaseOffset }));

	if (!pawnBase) return false;
	pawnBaseAddr = pawnBase;

	if (!UpdatePawn()) return false;

	return true;
}

bool Entity::SetEntName()
{
	const char* sEntNamePtr{ reinterpret_cast<char*>(cBaseAddr + MyOffset::Controller::cEntName) };
	if (!sEntNamePtr) return false;

	const std::string entNameBuffer(sEntNamePtr);
	cBase.sEntName = entNameBuffer;

	return true;
}

bool Entity::SetIsDormant()
{
	pawnBase.bDormant = MyPointers::ReadInternalMem<bool>(pawnBaseAddr,
		{ MyOffset::Pawn::pGameSceneNode, MyOffset::Pawn::bDormant });

	return true;
}

bool Entity::SetTeamNum()
{
	pawnBase.iTeamNum = MyPointers::ReadInternalMem<int32_t>(pawnBaseAddr, { MyOffset::Pawn::iTeamNum });
	return true;
}

bool Entity::SetvAngEyeAngle()
{
	pawnBase.vAngEyeAngle = *reinterpret_cast<Vector3*>(pawnBaseAddr + MyOffset::Pawn::vAngEyeAngles);
	return true;
}

bool Entity::SetvLastCameraPos()
{
	pawnBase.vLastCameraPos = *reinterpret_cast<Vector3*>(pawnBaseAddr + MyOffset::Pawn::vLastClipCameraPos);
	return true;
}

bool Entity::SetSpottedMask()
{
	pawnBase.bSpottedMask = MyPointers::ReadInternalMem<std::bitset<64>>(pawnBaseAddr, { MyOffset::Pawn::bSpottedMask });
	return true;
}

bool Entity::SetHealth()
{
	pawnBase.iHealth = MyPointers::ReadInternalMem<int32_t>(pawnBaseAddr, { MyOffset::Pawn::iHealth });
	return true;
}

intptr_t Entity::GetBoneArrayBase()
{
	return static_cast<intptr_t>(MyPointers::ReadInternalMem<uintptr_t>(pawnBaseAddr,
		{ MyOffset::Pawn::pGameSceneNode, MyOffset::Pawn::BoneArray }));
}

Vector3 Entity::GetBonePos(Bone pBone)
{
	BoneJoint* boneJointBase{ reinterpret_cast<BoneJoint*>(GetBoneArrayBase() + (pBone * sizeof(BoneJoint))) };
	if (!boneJointBase) return Vector3();

	return boneJointBase->pos;
}

Controller Entity::GetCBase()
{
	return cBase;
}

int Entity::GetIndex()
{
	return this->iIndex;
}

Pawn Entity::GetPawnBase()
{
	return pawnBase;
}

bool Entity::IsEntInit()
{
	return isEntInit;
}