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
	bool CompileShaders();
	bool SetConstantBuffer();
	void SetInputAssembler();
	void SetOrthoMatrix(D3D11_VIEWPORT pViewport);
	bool SetDeviceContext(IDXGISwapChain* pSwapchain);
	bool SetInputLayout(ID3D10Blob* pCompiledShaderBlob);
	void SetLineVBuffer(float x, float y, float x2, float y2, D3DCOLORVALUE pColor);
	void SetLineWHVBuffer(float x, float y, float x2, float y2, D3DCOLORVALUE pColor);
	void SetBoxVBuffer(float pX, float pY, float pWidth, float pHeight, D3DCOLORVALUE pColor);
	bool CompileShader(const char* szShader, const char* szEntrypoint, const char* szTarget, ID3D10Blob** pBlob);

public:
	ID3D11Device* m_device{ nullptr };
	IDXGISwapChain* m_swapChain{ nullptr };
	ID3D11DeviceContext* m_context{ nullptr };

	// Shaders
	ID3D11PixelShader* m_pixelShader{ nullptr };
	ID3D11VertexShader* m_vertexShader{ nullptr };

	// Input assembler
	ID3D11InputLayout* m_vInputLayout{ nullptr };
	ID3D11Buffer* m_vertexBuffer{ nullptr };
	ID3D11Buffer* m_constantBuffer{ nullptr };
	DirectX::XMMATRIX m_orthoMatrix;

	// Rasterizer
	D3D11_VIEWPORT m_viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{ 0 };
	D3D11_VIEWPORT m_viewport;
	HWND m_hwnd;
	RECT m_hRect;

	// Output Merger
	ID3D11RenderTargetView* m_renderTargetView { nullptr};

	// Hooking
	using TPresent = HRESULT(__stdcall*)(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags);
	void* o_Present;
	TPresent t_presentGateway;

	// Setup graphic pipeline
	void BeginDraw();
	bool InitDraw(IDXGISwapChain* pSwapchain);

	//  Drawing
	void DrawLine(float x, float y, float x2, float y2, D3DCOLORVALUE color);
	void DrawLineWH(float x, float y, float width, float height, D3DCOLORVALUE color); //uses 1 vertex + width and height
	void DrawBox(float x, float y, float width, float height, D3DCOLORVALUE color);
	void TestRender();

	void SafeRelease(auto* pData);
	~MyD3d11();
};