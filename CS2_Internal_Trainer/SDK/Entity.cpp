#include "Entity.h"

Entity::Entity(intptr_t pCBaseAddr)
	: cBase { static_cast<Controller>(pCBaseAddr) }

{
	if (pCBaseAddr != NULL)
	{
		if ((UpdateController(pCBaseAddr)))
			isEntInit = true;
	}
}

Entity::Entity()
{
}

bool Entity::UpdateController(intptr_t pCBaseAddr)
{
	if (!SetEntName(pCBaseAddr)) return false;
	if (!SetHPawn(pCBaseAddr)) return false;
	if (!SetPawnBase(pCBaseAddr)) return false;

	return true;
}

bool Entity::UpdatePawn(intptr_t pPawnAddr)
{
	if (!SetIsDormant(pPawnAddr)) return false;
	if (!SetHealth(pPawnAddr)) return false;
	if (!SetHeadPos(pPawnAddr)) return false;
	if (!SetPelvisPos(pPawnAddr)) return false;
	if (!SetSpottedMask(pPawnAddr)) return false;
	if (!SetTeamNum(pPawnAddr)) return false;
	if (!SetvAngEyeAngle(pPawnAddr)) return false;
	if (!SetvLastCameraPos(pPawnAddr)) return false;

	return true;
}

bool Entity::SetHPawn(intptr_t pCBaseAddr)
{
	cBase.hPawn = MyPointers::ReadInternalMem<int32_t>(pCBaseAddr, { MyOffset::Controller::hPawn });

	return cBase.hPawn ? true : false;
}

bool Entity::SetPawnBase(intptr_t pCBaseAddr)
{
	const intptr_t cGameEntity{ MyPointers::GetGameEntitySystemBase() + MyOffset::EntityListPtr};
	const int32_t pawnListOffset{ 8 * ((cBase.hPawn & 0x7FFF) >> 9) };
	const int32_t pawnBaseOffset{ 0x78 * (cBase.hPawn & 0x1FF) };

	intptr_t pawnBase{};

	if (pawnListOffset)
		pawnBase = static_cast<intptr_t>(MyPointers::ReadInternalMem<uintptr_t>(cGameEntity, { pawnListOffset, pawnBaseOffset }));
	else
		pawnBase = static_cast<intptr_t>(MyPointers::ReadInternalMem<uintptr_t>(*reinterpret_cast<intptr_t*>(cGameEntity), { pawnBaseOffset }));
	
	if (!pawnBase) return false;
	if (!UpdatePawn(pawnBase)) return false;

	return true;
}

bool Entity::SetEntName(intptr_t pCBaseAddr)
{
	const char* sEntNamePtr{ reinterpret_cast<char*>(pCBaseAddr + MyOffset::Controller::cEntName) };
	if (!sEntNamePtr) return false;

	const std::string entNameBuffer(sEntNamePtr);
	cBase.sEntName = entNameBuffer;

	return true;
}

bool Entity::SetIsDormant(intptr_t pPawnAddr)
{
	pawnBase.bDormant = MyPointers::ReadInternalMem<bool>(pPawnAddr,
		{MyOffset::Pawn::pGameSceneNode, MyOffset::Pawn::bDormant});

	return true;
}

bool Entity::SetTeamNum(intptr_t pPawnAddr)
{
	pawnBase.iTeamNum = MyPointers::ReadInternalMem<int32_t>(pPawnAddr, { MyOffset::Pawn::iTeamNum });
	return true;
}

bool Entity::SetvAngEyeAngle(intptr_t pPawnAddr)
{
	pawnBase.vAngEyeAngle = *reinterpret_cast<Vector3*>(pPawnAddr + MyOffset::Pawn::vAngEyeAngles);
	return true;
}

bool Entity::SetvLastCameraPos(intptr_t pPawnAddr)
{
	pawnBase.vLastCameraPos = *reinterpret_cast<Vector3*>(pPawnAddr + MyOffset::Pawn::vLastClipCameraPos);
	return true;
}

bool Entity::SetSpottedMask(intptr_t pPawnAddr)
{
	pawnBase.bSpottedMask =MyPointers::ReadInternalMem<std::bitset<64>>(pPawnAddr, { MyOffset::Pawn::bSpottedMask });
	return true;
}

bool Entity::SetHealth(intptr_t pPawnAddr)
{
	pawnBase.iHealth = MyPointers::ReadInternalMem<int32_t>(pPawnAddr, { MyOffset::Pawn::iHealth });
	return true;
}

intptr_t Entity::GetBoneArrayBase(intptr_t pPawnAddr)
{
	return static_cast<intptr_t>(MyPointers::ReadInternalMem<uintptr_t>(pPawnAddr,
		{MyOffset::Pawn::pGameSceneNode, MyOffset::Pawn::BoneArray}));
}

bool Entity::SetHeadPos(intptr_t pPawnAddr)
{
	BoneJoint* headBoneJoint{ reinterpret_cast<BoneJoint*>(GetBoneArrayBase(pPawnAddr) + (6 * sizeof(BoneJoint))) };
	if (!headBoneJoint) return false;

	pawnBase.headBonePos = headBoneJoint->pos;
	pawnBase.headBonePos.z -= 1.f; 

	return true;
}

bool Entity::SetPelvisPos(intptr_t pPawnAddr)
{
	BoneJoint* boneJointBase{ reinterpret_cast<BoneJoint*>(GetBoneArrayBase(pPawnAddr)) };
	if (!boneJointBase) return false;

	pawnBase.pelvisBonePos = boneJointBase->pos;
	return true;
}

Controller Entity::GetCBase()
{
	return cBase;
}

Pawn Entity::GetPawnBase()
{
	return pawnBase;
}

bool Entity::IsEntInit()
{
	return isEntInit;
}