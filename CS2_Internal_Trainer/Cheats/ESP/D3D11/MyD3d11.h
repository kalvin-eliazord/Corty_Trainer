#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "MyD3D_Utils.h"
#include "MyShaders.h"
#include "MyD3D11_VMT.h"
#include "Vector3.h"

class MyD3D11
{
private:
	bool SetViewport();
	bool CompileShaders();
	bool SetConstantBuffer();
	bool SetDeviceContextRenderTarget();
	void SetOrthoMatrix(D3D11_VIEWPORT pViewport);
	bool SetInputLayout(ID3D10Blob* pCompiledShaderBlob);
	void SetInputAssembler(D3D_PRIMITIVE_TOPOLOGY pPrimitiveTopology);
	void SetLineVBuffer(float x, float y, float x2, float y2, D3DCOLORVALUE pColor);
	void SetLineWHVBuffer(float x, float y, float x2, float y2, D3DCOLORVALUE pColor);
	void SetBoxVBuffer(float pX, float pY, float pWidth, float pHeight, D3DCOLORVALUE pColor);
	bool CompileShader(const char* szShader, const char* szEntrypoint, const char* szTarget, ID3D10Blob** pBlob);

public:
	ID3D11Device* m_device{ nullptr };
	IDXGISwapChain* m_swapChain{ nullptr };
	ID3D11DeviceContext* m_context{ nullptr };
	bool bDrawInit{ false };

	// Shaders
	ID3D11PixelShader* m_pixelShader{ nullptr };
	ID3D11VertexShader* m_vertexShader{ nullptr };

	// Input assembler
	ID3D11InputLayout* m_vInputLayout{ nullptr };
	ID3D11Buffer* m_vertexBuffer{ nullptr };
	ID3D11Buffer* m_constantBuffer{ nullptr };
	DirectX::XMMATRIX m_orthoMatrix{ nullptr };

	// Rasterizer
	D3D11_VIEWPORT m_viewport{};
	HWND m_hwnd{};
	RECT m_hRect{};

	// Output Merger
	ID3D11RenderTargetView* m_renderTargetView{ nullptr };

	// Hooking
	using T_Present = HRESULT(*)(IDXGISwapChain* pSwap, UINT pSyncInterval, UINT pFlags);
	T_Present t_presentGateway{};
	void* o_Present{};
	bool SetO_Present();

	// Setup graphic pipeline
	bool InitDraw(IDXGISwapChain* pSwapchain);
	HWND GetSwapHwnd(IDXGISwapChain* pSwapchain);

	//  Drawing
	void TestRender();
	void DrawLine(float x, float y, float x2, float y2, D3DCOLORVALUE color);
	void DrawBox(float x, float y, float width, float height, D3DCOLORVALUE color);
	void DrawLineWH(float x, float y, float width, float height, D3DCOLORVALUE color);

	void SafeRelease(auto*& pData);
	~MyD3D11();
};