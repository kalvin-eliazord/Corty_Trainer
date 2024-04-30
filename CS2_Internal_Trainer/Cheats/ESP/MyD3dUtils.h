#pragma once
#include <d3d11.h>
#include <DirectXColors.h>

const D3DCOLORVALUE red{ 1.0f, 0.0f, 0.0f, 1.0f };
const D3DCOLORVALUE green{ 0.0f, 1.0f, 0.0f, 1.0f };
const D3DCOLORVALUE blue{ 0.0f, 0.0f, 1.0f, 1.0f };
const D3DCOLORVALUE magenta{ 1.0f, 0.0f, 1.0f, 1.0f };
const D3DCOLORVALUE yellow{ 1.0f, 1.0f, 0.0f, 1.0f };

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	D3DCOLORVALUE color;
};

namespace MyD3dUtils
{
	HWND FindMainWindow(DWORD dwPID);
	BOOL CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam);
}

struct HandleData
{
	DWORD pid;
	HWND hWnd;
};
