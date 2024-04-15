#include "Entity.h"
#include <bitset>
#include "BoneJoint.h"

Entity::Entity(Controller* pSrc)
	: cBase(pSrc)
{
	if (cBase)
		SetPawnBase();
}

void Entity::SetPawnBase()
{
	intptr_t* cGameEntityBase{ reinterpret_cast<intptr_t*>(
	reinterpret_cast<intptr_t>(GamePointer::CGameEntityPtr) + 0x10) };

	const int32_t pawnListOffset{ 8 * ((cBase->pawnOffset & 0x7FFF) >> 9) };

	intptr_t* pawnListBase = *reinterpret_cast<intptr_t**>(reinterpret_cast<intptr_t>(cGameEntityBase) + pawnListOffset);

	if (pawnListBase)
	{
		const int32_t pawnOffset{ 0x78 * (cBase->pawnOffset & 0x1FF) };

		pawnBase = *reinterpret_cast<Pawn**>(reinterpret_cast<intptr_t>(pawnListBase) + pawnOffset);
	}

	IsPawnInit = (pawnBase ? true : false);
}

Controller* Entity::GetControllerBase()
{
	return cBase;
}

Pawn* Entity::GetPawnBase()
{
	return pawnBase;
}

bool Entity::GetIsPawnInit()
{
	return IsPawnInit;
}

std::bitset<64> Entity::GetSpottedId()
{
	intptr_t bSpottedIdAddr{ reinterpret_cast<intptr_t>(pawnBase) + GamePointer::Offset::bSpottedId };

	return *reinterpret_cast<std::bitset<64>*>(bSpottedIdAddr);
}

intptr_t* Entity::GetBoneArrayBase()
{
	intptr_t gameSceneNodePtr{ reinterpret_cast<intptr_t>(pawnBase) + GamePointer::Offset::GameSceneNode };

	gameSceneNodePtr = *reinterpret_cast<intptr_t*>(gameSceneNodePtr);

	return *reinterpret_cast<intptr_t**>(gameSceneNodePtr + GamePointer::Offset::BoneArray);
}

Vector3 Entity::GetHeadPos()
{
	Vector3 headBonePos;
	BoneJoint* boneJoinBase{ reinterpret_cast<BoneJoint*>(GetBoneArrayBase()) };

	BoneJoint* headBoneJoint{ reinterpret_cast<BoneJoint*>(reinterpret_cast<intptr_t>(boneJoinBase) + (6 * sizeof(BoneJoint))) };
	headBonePos = headBoneJoint->pos;

	return headBonePos;
}