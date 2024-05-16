#pragma once
#include <cstdint>

namespace Pointer
{
	inline float* lp_Yaw{ nullptr };
	inline float* lp_Pitch{ nullptr };
	inline int16_t* gameTypeId{ nullptr };
	inline int16_t* gameStateId{ nullptr };
	inline float* viewMatrix{ nullptr };
	inline intptr_t* cGameEntity{ nullptr };
	inline intptr_t* lp_Controller{ nullptr };
	inline intptr_t* entityListBase{ nullptr };
}