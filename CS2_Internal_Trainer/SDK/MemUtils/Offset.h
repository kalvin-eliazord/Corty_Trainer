#pragma once
#include <cstdint>

namespace Offset
{
	inline constexpr int32_t GameModeRules{ 0xD88 };
	inline constexpr int32_t bDormant{ 0xE7 }; // pawnBase + GameSceneNode + bDormant
	inline constexpr int32_t Lp_Yaw{ 0x5394 };
	inline constexpr int32_t EntityList{ 0x10 };
	inline constexpr int32_t GameTypeId{ 0x32 };
	inline constexpr int32_t Lp_Pitch{ 0x5390 };
	inline constexpr int32_t BoneArray{ 0x1E0 };
	inline constexpr int32_t GameStateId{ 0xA0 };
	inline constexpr int32_t bSpottedId{ 0x2278 + 0xC };
	inline constexpr int32_t GameSceneNode{ 0x318 }; // pawnBase + GameSceneNode
}