#include "Entity.h"

Entity::Entity(intptr_t* pContBase)
	: contBase(*(Controller**)pContBase) 
{
	if (this->SetPawnBase())
		this->isPawnInit = true;
}

bool Entity::SetPawnBase()
{
	if (!contBase) return false;

	this->pawnBase =  *(Pawn**)(
		reinterpret_cast<intptr_t>(GamePointer::entityListBasePtr) 
		+ 0x78 
		* (contBase->pawnOffset & 0x1FF)); // Thanks Aimstar for the inspiration

	if (!pawnBase) return false;

	return true;
}