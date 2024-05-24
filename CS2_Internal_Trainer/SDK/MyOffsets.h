#pragma once
#include <cstdint>
#include "offsets.hpp"
#include "client.dll.hpp"

namespace MyOffset
{
	inline constexpr intptr_t EntityListPtr{ 0x10 }; // GameEntitySystem + EntityListPtr
	inline constexpr intptr_t IsMatchStarted{ 0xA0 }; // Prediction + IsMatchStarted

	namespace GPointers
	{
		inline intptr_t ClientMod{};
		inline intptr_t Engine2Mod{};
		inline intptr_t GameRules{};
		inline intptr_t CCSGO_Input{};
		inline intptr_t LP_Controller{};
		inline intptr_t ViewMatrix{};
		inline intptr_t ViewAngles{};
		inline intptr_t GameEntitySyst{};
		inline intptr_t Prediction{};
	}

	namespace Pawn
	{
		inline constexpr intptr_t BoneArray{ cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState + cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecOrigin }; // GameSceneNode + BoneArray
		inline constexpr intptr_t iHealth{ cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth };
		inline constexpr intptr_t pGameSceneNode{ cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode };
		inline constexpr intptr_t iTeamNum{ cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum };
		inline constexpr intptr_t vLastClipCameraPos{ cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_vecLastClipCameraPos };
		inline constexpr intptr_t vAngEyeAngles { cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_angEyeAngles };
		inline constexpr intptr_t bSpottedMask{ cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask };
		inline constexpr intptr_t bDormant{ cs2_dumper::schemas::client_dll::CGameSceneNode::m_bDormant }; // GameSceneNode + bDormant
	};

	namespace Controller
	{
		inline constexpr intptr_t cEntName{ cs2_dumper::schemas::client_dll::CBasePlayerController::m_iszPlayerName };
		inline constexpr intptr_t hPawn {cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn };
	};

	namespace CCSGO_Input
	{
		inline constexpr intptr_t ViewAngles{ 0xB0 }; // CCSGOInput - ViewAngles
	}

	namespace CS_GameRules
	{
		inline constexpr intptr_t GameModeRules{ cs2_dumper::schemas::client_dll::C_CSGameRules::m_pGameModeRules };
		inline constexpr intptr_t DeathMatchPtr{ 0x30 }; // GameRules + GameModeRules + DeathMatchPtr
	};
}