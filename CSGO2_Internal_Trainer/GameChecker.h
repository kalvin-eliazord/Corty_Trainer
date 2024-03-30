#pragma once
#include "header.h"

namespace GameChecker
{
	int* gameStateIdPtr{ nullptr };
	int8_t* gameTypePtr{nullptr};
	int inline constexpr inGameId{ 8 };
	int inline constexpr notInGameId{ 4 };
};