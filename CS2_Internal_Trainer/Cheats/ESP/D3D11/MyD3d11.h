#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "HWindow.h"
#include "MyShaders.h"
#include "MyD3d11_VMT.h"

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

class MyD3d11
{
public:
	ID3D11Device* mDevice{ nullptr };
	ID3D11DeviceContext* mContext{ nullptr };
	IDXGISwapChain* mSwapChain{ nullptr}; // ->Present()

	ID3D11RenderTargetView* mRenderTargetView { nullptr};
	ID3D11InputLayout* mVertexLayout { nullptr};

	ID3D11PixelShader* mPixelShader { nullptr};
	ID3D11VertexShader* mVertexShader{ nullptr };

	ID3D11Buffer* mVertexBuffer { nullptr};
	ID3D11Buffer* mIndexBuffer { nullptr};
	ID3D11Buffer* mConstantBuffer { nullptr};

	HWND mHwnd;
	RECT mHwndRect;

	D3D11_VIEWPORT mViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{ 0 };
	D3D11_VIEWPORT myViewport;
	DirectX::XMMATRIX mOrtho;

	using TPresent = HRESULT(__stdcall*)(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags);
	void* oPresent;					
	TPresent tPresentGateway;

	bool Set_oPresent();

	// Graphic pipeline process
	bool CompileShader(const char* szShader, const char* szEntrypoint, const char* szTarget, ID3D10Blob** pBlob);
	bool CompileShaders();
	bool GetViewport();
	bool SetupOrtho();

	bool GetDeviceContextRenderTarget(IDXGISwapChain* pSwapchain);
	bool InitD3DDraw(IDXGISwapChain* pSwapchain);

	//  DRAW
	void BeginDraw();
	void DrawLine(float x, float y, float x2, float y2, D3DCOLORVALUE color);
	void DrawLineWH(float x, float y, float width, float height, D3DCOLORVALUE color); //uses 1 vertex + width and height
	void DrawBox(float x, float y, float width, float height, D3DCOLORVALUE color);
	void TestRender();

	void SafeRelease(auto* pData);
	~MyD3d11();
};