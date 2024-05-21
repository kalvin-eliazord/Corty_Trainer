#pragma once
#include <cstdint>
#include "offsets.hpp"
#include "client.dll.hpp"

namespace MyOffset
{
	inline constexpr intptr_t EntityListPtr{ 0x10 }; // GameEntitySystem + EntityListPtr
	inline constexpr int32_t IsMatchStarted{ 0xA0 }; // Prediction + IsMatchStarted

	struct Pawn
	{
		static const intptr_t BoneArray{ 0x1E0 };
		static const intptr_t iHealth{ cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth };
		static const intptr_t pGameSceneNode{ cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode };
		static const intptr_t iTeamNum{ cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum };
		static const intptr_t vLastClipCameraPos{ cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_vecvLastClipCameraPos };
		static const intptr_t vAngEyeAngles { cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_angEyeAngles };
		static const intptr_t bSpottedMask{ cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask };
		static const intptr_t bDormant{ cs2_dumper::schemas::client_dll::CGameSceneNode::m_bDormant };
	};

	struct Controller
	{
		static const intptr_t cEntName{ cs2_dumper::schemas::client_dll::CBasePlayerController::m_iszPlayerName };
		static const intptr_t hPawn {cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn };
	};

	struct CS_GameRules
	{
		static const intptr_t GameModeRules{ cs2_dumper::schemas::client_dll::C_CSGameRules::m_pGameModeRules };
	//	static const intptr_t bHasMatchStarted {cs2_dumper::schemas::client_dll::C_CSGameRules::m_bHasMatchStarted};
	};
}