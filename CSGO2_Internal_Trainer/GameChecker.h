#pragma once
#include "header.h"

namespace GameChecker
{
	static int* gameStateIdPtr{ nullptr };
	int inline constexpr inGameId{ 8 };
	int inline constexpr notInGameId{ 4 };
};