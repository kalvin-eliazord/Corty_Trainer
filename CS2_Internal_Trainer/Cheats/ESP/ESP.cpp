#include "ESP.h"

bool ESP::Start(const std::vector<Entity>& pTargets)
{
	if (pTargets.empty()) return false;
	auto cTargets{pTargets };

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