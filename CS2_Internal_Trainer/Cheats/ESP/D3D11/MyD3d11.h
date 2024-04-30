#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "MyD3dUtils.h"
#include "MyShaders.h"
#include "MyD3d11_VMT.h"

class MyD3d11
{
private:
	bool SetViewport();
	void SetOrthoMatrix(D3D11_VIEWPORT pViewport);
	bool SetDeviceContext(IDXGISwapChain* pSwapchain);
	bool SetInputLayout(ID3D10Blob* pCompiledShaderBlob);

public:
	ID3D11Device* m_device{ nullptr };
	ID3D11DeviceContext* m_context{ nullptr };
	IDXGISwapChain* m_swapChain{ nullptr }; // ->Present()

	// Shadering
	ID3D11PixelShader* m_pixelShader{ nullptr };
	ID3D11VertexShader* m_vertexShader{ nullptr };
	ID3D11InputLayout* m_vInputLayout{ nullptr };
	ID3D11Buffer* m_vertexBuffer{ nullptr };
	ID3D11Buffer* m_constantBuffer{ nullptr };

	//ID3D11RenderTargetView* m_renderTargetView { nullptr}; UNUSED -> USING THE RENDER TARGET OF THE GAME

//	ID3D11Buffer* m_IndexBuffer { nullptr}; UNUSED -> optimizing rendering purpose

	HWND m_hwnd;
	RECT m_hRect;

	D3D11_VIEWPORT m_viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{ 0 };
	D3D11_VIEWPORT m_viewport;
	DirectX::XMMATRIX m_orthoMatrix;

	// Hooking
	using TPresent = HRESULT(__stdcall*)(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags);
	void* o_Present;
	TPresent t_presentGateway;
	bool Set_oPresent(); // UNUSED = patternScanning steamOverlay to get the addr of present

	// Setup for graphic pipeline
	void BeginDraw();
	bool CompileShaders();
	bool SetConstantBuffer();
	bool InitDraw(IDXGISwapChain* pSwapchain);
	bool CompileShader(const char* szShader, const char* szEntrypoint, const char* szTarget, ID3D10Blob** pBlob);

	//  DRAW
	void DrawLine(float x, float y, float x2, float y2, D3DCOLORVALUE color);
	void DrawLineWH(float x, float y, float width, float height, D3DCOLORVALUE color); //uses 1 vertex + width and height
	void DrawBox(float x, float y, float width, float height, D3DCOLORVALUE color);
	void TestRender();

	void SafeRelease(auto* pData);
	~MyD3d11();
};