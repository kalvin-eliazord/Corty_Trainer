#include "MyD3D_Utils.h"

HWND MyD3D_Utils::FindMainWindow(DWORD dwPID)
{
	WindowData wndData{};
	wndData.wndId = dwPID;
	EnumWindows(EnumWindowsCallback, (LPARAM)&wndData);

	return wndData.hWnd;
}

BOOL CALLBACK MyD3D_Utils::EnumWindowsCallback(HWND pHwnd, LPARAM lParam)
{
	WindowData& wndData{ *(WindowData*)lParam };
	DWORD currWndId{ 0 };
	GetWindowThreadProcessId(pHwnd, &currWndId);

	if (currWndId == wndData.wndId && GetWindow(pHwnd, GW_OWNER) == HWND(0) && IsWindowVisible(pHwnd))
	{
		wndData.hWnd = pHwnd;
		return FALSE;
	}

	return TRUE;
}

bool MyD3D_Utils::WorldToScreen(Vector3 pWorldPos, Vector3& pScreenPos, float* pMatrixPtr, const FLOAT pWinWidth, const FLOAT pWinHeight)
{
	float matrix2[4][4];

	memcpy(matrix2, pMatrixPtr, 16 * sizeof(float));

	const float mX{ pWinWidth / 2 };
	const float mY{ pWinHeight / 2 };

	const float w {
		matrix2[3][0] * pWorldPos.x +
		matrix2[3][1] * pWorldPos.y +
		matrix2[3][2] * pWorldPos.z +
		matrix2[3][3] };

	if (w < 0.65f) return false;

	const float x{
		matrix2[0][0] * pWorldPos.x +
		matrix2[0][1] * pWorldPos.y +
		matrix2[0][2] * pWorldPos.z +
		matrix2[0][3] };

	const float y {
		matrix2[1][0] * pWorldPos.x +
		matrix2[1][1] * pWorldPos.y +
		matrix2[1][2] * pWorldPos.z +
		matrix2[1][3] };

	pScreenPos.x = (mX + mX * x / w);
	pScreenPos.y = (mY - mY * y / w);
	pScreenPos.z = 0;

	return true;
}