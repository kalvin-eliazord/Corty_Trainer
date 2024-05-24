#include "ESP.h"

std::vector<Entity> ESP::GetValidTargets()
{
	std::vector<Entity> cTargets{};

	for (int i{ 0 }; i < 64; ++i)
	{
		Entity currEntity(MyPointers::GetEntityBase(i));

		if (!IsGoodTarget(&currEntity))
			continue;

		cTargets.push_back(currEntity);
	}

	return cTargets;
}

bool ESP::IsGoodTarget(Entity* pCurrEntPtr)
{
	if (!pCurrEntPtr->IsEntInit())
		return false;

	if (!LocalPlayer::GetEntity().IsEntInit()) return false;
	Pawn lpPawn{ LocalPlayer::GetPawn() };

	Pawn currEntPawn{ pCurrEntPtr->GetPawnBase() };

	if (LocalPlayer::GetController().sEntName == pCurrEntPtr->GetCBase().sEntName)
		return false;

	if (currEntPawn.iHealth < 1)
		return false;

	if (currEntPawn.bDormant)
		return false;
	
	if (CheatHKeys::bTeamCheck)
	{
		if (lpPawn.iTeamNum == currEntPawn.iTeamNum)
			return false;
	}

	return true;
}

bool ESP::SnapLineTo(Vector3 pEntPos, float pWinWidth, float pWinHeight)
{
	Vector3 currEntScreenPos{};
	if (!MyD3D_Utils::WorldToScreen(pEntPos, currEntScreenPos, MyPointers::GetViewMatrixPtr(), pWinWidth, pWinHeight))
		return false;

	g_myD3d11.DrawLine(currEntScreenPos.x, currEntScreenPos.y, pWinWidth/2, pWinHeight, MyD3D_Utils::blue);

	return true;
}

bool ESP::Start()
{
	std::vector<Entity> cTargets{ GetValidTargets() };
	if (cTargets.empty()) return false;

	const float winWidth{ g_myD3d11.m_viewport.Width };
	const float winHeight{ g_myD3d11.m_viewport.Height };

	for (auto& currTarget : cTargets)
	{
		Vector3 currEntPos { currTarget.GetPawnBase().vLastCameraPos};

		if (!SnapLineTo(currEntPos, winWidth, winHeight))
			continue;
	}

	return true;
}