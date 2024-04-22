#pragma once
#include <cstdint>

namespace Pointer
{
	inline float* lp_Yaw{};
	inline float* lp_Pitch{};
	inline int16_t* gameTypeId{};
	inline int16_t* gameStateId{};
	inline intptr_t* cGameEntity{};
	inline intptr_t* steamOverlay{};
	inline intptr_t* lp_Controller{};
	inline intptr_t* entityListBase{};
}