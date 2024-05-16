#include "ESP.h"

std::vector<Controller*> ESP::GetCTargets()
{
	intptr_t* entListBasePtr{ Pointer::entityListBase };
	std::vector<Controller*> cTargetsEntities{};

	for (int i{ 0 }; i < 64; ++i)
	{
		Entity currEntity(*reinterpret_cast<Controller**>(
			reinterpret_cast<intptr_t>(entListBasePtr)
			+ (i + 1)
			* 0x78));

		if (!IsGoodTarget(&currEntity))
			continue;

		cTargetsEntities.push_back(currEntity.GetControllerBase());
	}

	return cTargetsEntities;
}

bool ESP::IsGoodTarget(Entity* pCurrEntPtr)
{
	if (!pCurrEntPtr->GetIsPawnInit())
		return false;

	Pawn* lpPawn{ LocalPlayer::GetPawn() };
	if (!lpPawn) return false;

	Pawn* currEntPawn{ pCurrEntPtr->GetPawnBase() };

	if (lpPawn == currEntPawn)
		return false;

	// Entity dead
	if (currEntPawn->health < 1)
		return false;

	if (pCurrEntPtr->IsDormant())
		return false;
	
	if (CheatHKeys::bTeamCheck)
	{
		if (lpPawn->team_variable == currEntPawn->team_variable)
			return false;
	}

	// No Team check when there is no team UNUSED
	/*if (!IsGameDeathMatch(Pointer::gameTypeId))
	{
		if (lpPawn->team_variable == currEntPawn->team_variable)
			return false;
	}*/

	return true;
}

bool ESP::Start()
{
	std::vector<Controller*> cTargets{ GetCTargets() };
	if (cTargets.empty()) return false;

	const float winWidth{ g_myD3d11.m_viewport.Width };
	const float winHeight{ g_myD3d11.m_viewport.Height };

	for (const auto& currTarget : cTargets)
	{
		Entity currEnt(currTarget);
		
		Vector3 currEntPos { currEnt.GetPawnBase()->lastClipCameraPos };

		Vector3 currEntScreenPos{};
		if (!MyD3D_Utils::WorldToScreen(currEntPos, currEntScreenPos, Pointer::viewMatrix, winWidth, winHeight))
			break;

		g_myD3d11.DrawLine(currEntScreenPos.x, currEntScreenPos.y, winWidth/2, winHeight/2, MyD3D_Utils::blue);
	}

	return true;
}
