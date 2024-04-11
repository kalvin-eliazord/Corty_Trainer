#include "Entity.h"

Entity::Entity(intptr_t* pContBase)
	: controllerBase(*reinterpret_cast<Controller**>(pContBase))
{
	if(this->controllerBase)
		this->SetPawnBase();
}

void Entity::SetPawnBase()
{
	constexpr int paddingBetweenEnt{ 0x78 };

	this->pawnBase = *reinterpret_cast<Pawn**>(
		reinterpret_cast<intptr_t>(GamePointer::entityListBasePtr)
		+ paddingBetweenEnt
		* (controllerBase->pawnOffset & 0x1FF)); // Thanks @Aimstar

	this->IsPawnInit = (pawnBase ? true : false);
}

Pawn* Entity::GetPawnBase()
{
	return this->pawnBase;
}

bool Entity::GetIsPawnInit()
{
	return this->IsPawnInit;
}

Controller* Entity::GetControllerBase()
{
	return this->controllerBase;
}