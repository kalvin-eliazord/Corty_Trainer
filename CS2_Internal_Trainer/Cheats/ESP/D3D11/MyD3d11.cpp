#include "MyD3D11.h"

MyD3D11::~MyD3D11()
{
	SafeRelease(m_constantBuffer);
	SafeRelease(m_vertexBuffer);
	SafeRelease(m_vInputLayout);
	SafeRelease(m_vertexShader);
	SafeRelease(m_pixelShader);
}

void MyD3D11::SafeRelease(auto*& pData)
{
	if (pData)
	{
		pData->Release();
		pData = nullptr;
	}
}

bool MyD3D11::SetInputLayout(ID3D10Blob* pCompiledShaderBlob)
{
	const D3D11_INPUT_ELEMENT_DESC layout[2] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const UINT numElements{ ARRAYSIZE(layout) };

	const HRESULT hRes{ m_device->CreateInputLayout(layout, numElements, pCompiledShaderBlob->GetBufferPointer(), pCompiledShaderBlob->GetBufferSize(), &m_vInputLayout) };

	if (FAILED(hRes)) return false;

	return true;
}

void MyD3D11::SetOrthoMatrix(D3D11_VIEWPORT pViewport)
{
	m_orthoMatrix = DirectX::XMMatrixOrthographicOffCenterLH(0, pViewport.Width, pViewport.Height, 0, 0.0f, 1.0f);
}

bool MyD3D11::CompileShader(const char* szShader, const char* szEntrypoint, const char* szTarget, ID3D10Blob** compiledShaderBlob)
{
	ID3D10Blob* pErrorBlob{ nullptr };

	const auto hRes{ D3DCompile(szShader, strlen(szShader), 0, nullptr, nullptr, szEntrypoint, szTarget, D3DCOMPILE_ENABLE_STRICTNESS, 0, compiledShaderBlob, &pErrorBlob) };
	if (FAILED(hRes))
	{
		if (pErrorBlob)
		{
			char szError[256]{ 0 };
			memcpy(szError, pErrorBlob->GetBufferPointer(), pErrorBlob->GetBufferSize());
			MessageBoxA(nullptr, szError, "Error", MB_OK);
		}
		return false;
	}
	return true;
}

bool MyD3D11::CompileShaders()
{
	ID3D10Blob* compiledShaderBlob{ nullptr };

	// Vertex shader
	if (!CompileShader(shaders, "VS", "vs_5_0", &compiledShaderBlob))
		return false;

	HRESULT hRes{ m_device->CreateVertexShader(compiledShaderBlob->GetBufferPointer(), compiledShaderBlob->GetBufferSize(), nullptr, &m_vertexShader) };
	if (FAILED(hRes)) return false;

	// Sets the input layout of the vertex buffer
	if (!SetInputLayout(compiledShaderBlob))
		return false;

	SafeRelease(compiledShaderBlob);

	// Pixel shader
	if (!CompileShader(shaders, "PS", "ps_5_0", &compiledShaderBlob))
		return false;

	hRes = m_device->CreatePixelShader(compiledShaderBlob->GetBufferPointer(), compiledShaderBlob->GetBufferSize(), nullptr, &m_pixelShader);
	if (FAILED(hRes)) return false;

	return true;
}

bool MyD3D11::SetViewport()
{
	m_hwnd = GetSwapHwnd(m_swapChain);

	if (!GetClientRect(m_hwnd, &m_hRect))
		return false;

	// Setup viewport
	m_viewport.Width = static_cast<FLOAT>(m_hRect.right);
	m_viewport.Height = static_cast<FLOAT>(m_hRect.bottom);
	m_viewport.TopLeftX = static_cast<FLOAT>(m_hRect.left);
	m_viewport.TopLeftY = static_cast<FLOAT>(m_hRect.top);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	return true;
}

bool MyD3D11::SetConstantBuffer()
{
	SetOrthoMatrix(m_viewport);

	D3D11_BUFFER_DESC buffer_desc{ 0 };
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.ByteWidth = sizeof(DirectX::XMMATRIX);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subResourceLine{ 0 };
	subResourceLine.pSysMem = &m_orthoMatrix;

	HRESULT hRes{ m_device->CreateBuffer(&buffer_desc, &subResourceLine, &m_constantBuffer) };
	if (FAILED(hRes)) return false;

	return true;
}

bool MyD3D11::SetDeviceContextRenderTarget()
{
	// Get game device
	HRESULT hRes{ m_swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&m_device) };
	if (FAILED(hRes)) return false;

	m_device->GetImmediateContext(&m_context);

	// Init Render Target View
	ID3D11Texture2D* backBuffer{ nullptr };
	hRes = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(hRes)) return false;

	hRes = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
	backBuffer->Release();
	if (FAILED(hRes)) return false;

	return true;
}

bool MyD3D11::InitDraw(IDXGISwapChain* pSwapchain)
{
	m_swapChain = pSwapchain;

	if (!SetDeviceContextRenderTarget()) return false;

	if (!CompileShaders()) return false;

	if (!SetViewport()) return false;

	if (!SetConstantBuffer()) return false;

	return true;
}

bool MyD3D11::SetO_Present()
{
	DXGI_SWAP_CHAIN_DESC sd{ 0 };
	sd.BufferCount = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.OutputWindow = MyD3D_Utils::FindMainWindow(GetCurrentProcessId());
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.SampleDesc.Count = 1;

	ID3D11Device* device{ nullptr };
	IDXGISwapChain* swapChain{ nullptr };

	const HRESULT hRes{ D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&swapChain,
		&device,
		nullptr,
		nullptr) };

	if (FAILED(hRes)) return false;

	void** pVMT{ *reinterpret_cast<void***>(swapChain) };

	// Get Present's address out of vmt
	o_Present = (T_Present)(pVMT[(UINT)IDXGISwapChainVMT::Present]);

	SafeRelease(swapChain);
	SafeRelease(device);

	return true;
}

void MyD3D11::SetInputAssembler(D3D_PRIMITIVE_TOPOLOGY pPrimitiveTopology)
{
	const UINT stride{ sizeof(Vertex) };
	const UINT offset{ 0 };

	m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_context->IASetInputLayout(m_vInputLayout);
	m_context->IASetPrimitiveTopology(pPrimitiveTopology);
}

void MyD3D11::SetLineVBuffer(float x, float y, float x2, float y2, D3DCOLORVALUE pColor)
{
	Vertex vertices[2] = {
	{ DirectX::XMFLOAT3(x, y, 1.0f), pColor },
	{ DirectX::XMFLOAT3(x2, y2, 1.0f),	pColor },
	};

	D3D11_BUFFER_DESC buffer_desc_line{ 0 };
	buffer_desc_line.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc_line.ByteWidth = sizeof(Vertex) * 2;
	buffer_desc_line.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subResourceLine{ 0 };
	subResourceLine.pSysMem = &vertices;

	m_device->CreateBuffer(&buffer_desc_line, &subResourceLine, &m_vertexBuffer);
}

void MyD3D11::DrawLine(float x, float y, float x2, float y2, D3DCOLORVALUE pColor)
{
	// Setup vertices
	SetLineVBuffer(x, y, x2, y2, pColor);

	// Input Assembler
	SetInputAssembler(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Vertex Shader
	m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_context->VSSetShader(m_vertexShader, nullptr, 0);

	// Pixel Shader
	m_context->PSSetShader(m_pixelShader, nullptr, 0);

	// Output Merger
	m_context->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

	// Rasterizer
	m_context->RSSetViewports(1, &m_viewport);

	m_context->Draw(2, 0);
}

void MyD3D11::SetLineWHVBuffer(float pX, float pY, float pWidth, float pHeight, D3DCOLORVALUE pColor)
{
	Vertex vertices[2] = {
		{ DirectX::XMFLOAT3(pX,			   pY,			 1.0f),	pColor },
		{ DirectX::XMFLOAT3(pX + pWidth,   pY + pHeight, 1.0f),	pColor },
	};

	D3D11_BUFFER_DESC buffer_desc_line{ 0 };
	buffer_desc_line.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc_line.ByteWidth = sizeof(Vertex) * 2;
	buffer_desc_line.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subResourceLine{ 0 };
	subResourceLine.pSysMem = &vertices;

	m_device->CreateBuffer(&buffer_desc_line, &subResourceLine, &m_vertexBuffer);
}

void MyD3D11::DrawLineWH(float pX, float pY, float pWidth, float pHeight, D3DCOLORVALUE pColor)
{
	// Setup vertices
	SetLineWHVBuffer(pX, pY, pWidth, pHeight, pColor);

	// Input Assembler
	SetInputAssembler(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Vertex Shader
	m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_context->VSSetShader(m_vertexShader, nullptr, 0);

	// Pixel Shader
	m_context->PSSetShader(m_pixelShader, nullptr, 0);

	// Output Merger
	m_context->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

	// Rasterizer
	m_context->RSSetViewports(1, &m_viewport);

	m_context->Draw(2, 0);
}

void MyD3D11::SetBoxVBuffer(float pX, float pY, float pWidth, float pHeight, D3DCOLORVALUE pColor)
{
	Vertex vertices[5] = {
		{ DirectX::XMFLOAT3(pX,				pY,			  1.0f),		pColor },
		{ DirectX::XMFLOAT3(pX + pWidth,	pY,			  1.0f),		pColor },
		{ DirectX::XMFLOAT3(pX + pWidth,	pY + pHeight, 1.0f),		pColor },
		{ DirectX::XMFLOAT3(pX,				pY + pHeight, 1.0f),		pColor },
		{ DirectX::XMFLOAT3(pX,				pY,			  1.0f),		pColor }
	};

	D3D11_BUFFER_DESC buffer_desc_line{ 0 };
	buffer_desc_line.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc_line.ByteWidth = sizeof(Vertex) * 5;
	buffer_desc_line.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subResourceLine{ 0 };
	subResourceLine.pSysMem = &vertices;

	m_device->CreateBuffer(&buffer_desc_line, &subResourceLine, &m_vertexBuffer);
}

void MyD3D11::DrawBox(float pX, float pY, float pWidth, float pHeight, D3DCOLORVALUE pColor)
{
	// Setup vertices TL, TR, BR, BL
	SetBoxVBuffer(pX, pY, pWidth, pHeight, pColor);

	// Input Assembler
	SetInputAssembler(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// Vertex Shader
	m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_context->VSSetShader(m_vertexShader, nullptr, 0);

	// Pixel Shader
	m_context->PSSetShader(m_pixelShader, nullptr, 0);

	// Output Merger
	m_context->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

	// Rasterizer
	m_context->RSSetViewports(1, &m_viewport);

	m_context->Draw(5, 0);
}

HWND MyD3D11::GetSwapHwnd(IDXGISwapChain* pSwapchain)
{
	DXGI_SWAP_CHAIN_DESC swapDesc;
	pSwapchain->GetDesc(&swapDesc);

	return swapDesc.OutputWindow;
}

void MyD3D11::TestRender()
{
	//Draw from top left to bottom right if the viewport is correctly setup
	DrawLine(0, 0, 640, 360, MyD3D_Utils::green);
	DrawLine(0, 0, 640, -360, MyD3D_Utils::red);
	DrawLine(0, 0, -640, 360, MyD3D_Utils::magenta);
	DrawLine(0, 0, -640, -360, MyD3D_Utils::yellow);
	DrawBox(0, 0, 50, 100, MyD3D_Utils::green);
}