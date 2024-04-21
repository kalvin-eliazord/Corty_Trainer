#pragma once
#include <Windows.h>

namespace HWindow
{
	inline HWND hWindow{};
	inline long rWinWidth{};
	inline long rWinHeight{};

	BOOL CALLBACK enumWndCallBack(HWND enumHwin, LPARAM lp);
	HWND GetProcessWindow();
	bool SetRectWindow();
};