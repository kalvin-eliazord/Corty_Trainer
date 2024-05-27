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

	if (ConsoleMenu::bTeamCheck && lpPawn.iTeamNum == currEntPawn.iTeamNum)
		return false;

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
		Vector3 currBotPos{ currTarget.GetBonePos(Bone::ankle_L) };
		Vector3 currTopPos{ currTarget.GetBonePos(Bone::head) };

		Vector3 curr2DBot{};
		Vector3 curr2DTop{};

		if (!MyD3D_Utils::WorldToScreen(currBotPos, curr2DBot, MyPointers::GetViewMatrixPtr(), winWidth, winHeight))
			continue;

		if (!MyD3D_Utils::WorldToScreen(currTopPos, curr2DTop, MyPointers::GetViewMatrixPtr(), winWidth, winHeight))
			continue;

		// SnapLine
		g_myD3d11.DrawLine(curr2DBot.x, curr2DBot.y, winWidth / 2, winHeight, MyD3D_Utils::magenta);

		// Boxe
		const float height{ ::abs(curr2DTop.y - curr2DBot.y) * 1.25f };
		const float width{ height / 2.f };
		g_myD3d11.DrawBox(curr2DTop.x - (width / 2.f), curr2DTop.y - (width / 2.5f), width, height, MyD3D_Utils::magenta);
	}

	return true;
}