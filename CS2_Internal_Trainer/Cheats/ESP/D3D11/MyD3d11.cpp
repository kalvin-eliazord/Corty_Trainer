#include "MyD3d11.h"

void MyD3d11::SafeRelease(auto* pData)
{
	if (pData)
	{
		pData->Release();
		pData = nullptr;
	}
}

bool MyD3d11::SetInputLayout(ID3D10Blob* pCompiledShaderBlob)
{
	const D3D11_INPUT_ELEMENT_DESC layout[2] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const UINT numElements{ ARRAYSIZE(layout) };

	const HRESULT hRes{ m_device->CreateInputLayout(layout, numElements, pCompiledShaderBlob->GetBufferPointer(), pCompiledShaderBlob->GetBufferSize(), &m_vInputLayout) };

	if (FAILED(hRes)) return false;
}

void MyD3d11::SetOrthoMatrix(D3D11_VIEWPORT pViewport)
{
	m_orthoMatrix = DirectX::XMMatrixOrthographicOffCenterLH(0, pViewport.Width, pViewport.Height, 0, 0.0f, 1.0f);
}

bool MyD3d11::Set_oPresent()
{
	DXGI_SWAP_CHAIN_DESC sDesc{ 0 };
	sDesc.BufferCount = 1;
	sDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sDesc.OutputWindow = GetDesktopWindow();
	sDesc.Windowed = TRUE;
	sDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sDesc.SampleDesc.Count = 1;

	ID3D11Device* device{ nullptr };
	IDXGISwapChain* swapchain{ nullptr };

	HRESULT hResult{ D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		NULL, nullptr,
		NULL,
		D3D11_SDK_VERSION,
		&sDesc,
		&swapchain,
		&device,
		nullptr,
		nullptr) };

	if (GetLastError() == 0x594) SetLastError(0); // Ignore error related to output window //TODO: fix this little issue with window handle

	if (FAILED(hResult)) return false;

	void** swapVMT{ *(void***)swapchain };

	// Get Present's address out of VMT
	o_Present = static_cast<TPresent>(swapVMT[static_cast<UINT>(IDXGISwapChainVMT::Present)]);

	SafeRelease(swapchain);
	SafeRelease(device);

	return true;
}

bool MyD3d11::CompileShader(const char* szShader, const char* szEntrypoint, const char* szTarget, ID3D10Blob** compiledShaderBlob)
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

bool MyD3d11::CompileShaders()
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

	//m_context->IASetInputLayout(m_vInputLayout);

	SafeRelease(compiledShaderBlob);

	// Pixel shader
	if (!CompileShader(shaders, "PS", "ps_5_0", &compiledShaderBlob))
		return false;

	hRes = m_device->CreatePixelShader(compiledShaderBlob->GetBufferPointer(), compiledShaderBlob->GetBufferSize(), nullptr, &m_pixelShader);
	if (FAILED(hRes)) return false;

	return true;
}

bool MyD3d11::SetViewport()
{
	UINT numViewports{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };

	m_context->RSGetViewports(&numViewports, m_viewports);

	if (!numViewports || !m_viewports[0].Width)
	{
		m_hwnd = MyD3dUtils::FindMainWindow(GetCurrentProcessId());

		if (!GetClientRect(m_hwnd, &m_hRect))
			return false;

		// Setup viewport
		m_viewport.Width = static_cast<FLOAT>(m_hRect.right);
		m_viewport.Height = static_cast<FLOAT>(m_hRect.bottom);
		m_viewport.TopLeftX = static_cast<FLOAT>(m_hRect.left);
		m_viewport.TopLeftY = static_cast<FLOAT>(m_hRect.top);
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_context->RSSetViewports(1, &m_viewport);

		return true;
	}

	m_viewport = m_viewports[0];
	m_context->RSSetViewports(1, &m_viewport);

	return true;
}

bool MyD3d11::SetConstantBuffer()
{
	SetOrthoMatrix(m_viewport);

	D3D11_BUFFER_DESC buffer_desc{ 0 };
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.ByteWidth = sizeof(m_orthoMatrix);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subResourceLine{ 0 };
	subResourceLine.pSysMem = &m_orthoMatrix;

	HRESULT hRes{ m_device->CreateBuffer(&buffer_desc, &subResourceLine, &m_constantBuffer) };
	if (FAILED(hRes)) return false;

	// m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer); UNUSED -> DONE ON DRAWING

	return true;
}

bool MyD3d11::SetDeviceContext(IDXGISwapChain* pSwapchain)
{
	// Get game device
	HRESULT hRes{ pSwapchain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&m_device)) };
	if (FAILED(hRes)) return false;

	m_device->GetImmediateContext(&m_context);

	// UNUSED -> USING THE RENDER TARGET OF THE GAME
	//	m_context->OMGetRenderTargets(1, &m_renderTargetView, nullptr);

	// m_renderTargetView backup
	/*if (!m_renderTargetView)
	{
		ID3D11Texture2D* backbuffer{ nullptr };
		hRes = pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer));
		if (FAILED(hRes)) return false;

		hRes = m_device->CreateRenderTargetView(backbuffer, nullptr, &m_renderTargetView);
		backbuffer->Release();
		if (FAILED(hRes)) return false;

		m_context->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
	}*/

	return true;
}

bool MyD3d11::InitDraw(IDXGISwapChain* pSwapchain)
{
	m_swapChain = pSwapchain;

	if (!SetDeviceContext(pSwapchain)) return false;

	if (!CompileShaders()) return false;

	if (!SetViewport()) return false;

	if (!SetConstantBuffer()) return false;

	return true;
}

void MyD3d11::BeginDraw()
{
	SetViewport();
	SetConstantBuffer();
}

void MyD3d11::DrawLine(float x, float y, float x2, float y2, D3DCOLORVALUE color)
{
	//start SET for vertexShader
	// Setup vertices
	Vertex vertexs[2] = {
		{ DirectX::XMFLOAT3(x, y, 1.0f), color },
		{ DirectX::XMFLOAT3(x2, y2, 1.0f),	color },
	};

	D3D11_BUFFER_DESC buffer_desc_line{ 0 };
	buffer_desc_line.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc_line.ByteWidth = sizeof(Vertex) * 2;
	buffer_desc_line.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subResourceLine{ 0 };
	subResourceLine.pSysMem = &vertexs;

	m_device->CreateBuffer(&buffer_desc_line, &subResourceLine, &m_vertexBuffer);

	m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);

	UINT stride{ sizeof(Vertex) };
	UINT offset{ 0 };

	m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_context->IASetInputLayout(m_vInputLayout);
	//end SET for vertexShader

	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); // IDK 

	m_context->VSSetShader(m_vertexShader, nullptr, 0);
	m_context->PSSetShader(m_pixelShader, nullptr, 0);

	//m_context->RSSetViewports(1, &m_viewport);

	m_context->Draw(2, 0);
}

void MyD3d11::DrawLineWH(float x, float y, float width, float height, D3DCOLORVALUE color)
{
	// Setup vertices
	Vertex vertexs[2] = {
	{ DirectX::XMFLOAT3(x,			 y,			 1.0f),	color },
	{ DirectX::XMFLOAT3(x + width,   y + height, 1.0f),	color },
	};

	D3D11_BUFFER_DESC buffer_desc_line{ 0 };
	buffer_desc_line.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc_line.ByteWidth = sizeof(Vertex) * 2;
	buffer_desc_line.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subResourceLine{ 0 };
	subResourceLine.pSysMem = &vertexs;

	m_device->CreateBuffer(&buffer_desc_line, &subResourceLine, &m_vertexBuffer);

	m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);

	// Make sure the input assembler knows how to process our verts/indices
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_context->IASetInputLayout(m_vInputLayout);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Set the shaders we need to render our line
	m_context->VSSetShader(m_vertexShader, nullptr, 0);
	m_context->PSSetShader(m_pixelShader, nullptr, 0);

	// Set viewport to context, unnecessary so far
	//m_context->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	//m_context->RSSetViewports(1, &m_viewport);

	m_context->Draw(2, 0);
}

void MyD3d11::DrawBox(float x, float y, float width, float height, D3DCOLORVALUE color)
{
	// Setup vertices TL, TR, BR, BL

	Vertex vertexs[5] = {
		{ DirectX::XMFLOAT3(x,			y,			1.0f),		color },
		{ DirectX::XMFLOAT3(x + width,	y,			1.0f),		color },
		{ DirectX::XMFLOAT3(x + width,	y + height, 1.0f),		color },
		{ DirectX::XMFLOAT3(x,			y + height, 1.0f),		color },
		{ DirectX::XMFLOAT3(x,			y,			1.0f),		color }
	};

	D3D11_BUFFER_DESC buffer_desc_line{ 0 };
	buffer_desc_line.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc_line.ByteWidth = sizeof(Vertex) * 5;
	buffer_desc_line.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subResourceLine{ 0 };
	subResourceLine.pSysMem = &vertexs;

	m_device->CreateBuffer(&buffer_desc_line, &subResourceLine, &m_vertexBuffer);

	m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);

	// Make sure the input assembler knows how to process our verts/indices
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_context->IASetInputLayout(m_vInputLayout);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// Set the shaders we need to render our line
	m_context->VSSetShader(m_vertexShader, nullptr, 0);
	m_context->PSSetShader(m_pixelShader, nullptr, 0);

	// Set viewport to context, unnecessary so far?
	//m_context->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	//m_context->RSSetViewports(1, &m_viewport);

	m_context->Draw(5, 0);
}

void MyD3d11::TestRender()
{
	//this easily lets you debug viewport and ortho matrix
	//it will draw from top left to bottom right if your viewport is correctly setup
	DrawLine(0, 0, 640, 360, green);
	DrawLine(0, 0, 640, -360, red);
	DrawLine(0, 0, -640, 360, magenta);
	DrawLine(0, 0, -640, -360, yellow);
	DrawBox(0, 0, 50, 100, green); // TL, TR, BR, BL
}

MyD3d11::~MyD3d11()
{
	SafeRelease(m_constantBuffer);
	SafeRelease(m_vertexBuffer);
	//SafeRelease(m_IndexBuffer); UNUSED
	SafeRelease(m_vInputLayout);
	SafeRelease(m_vertexShader);
	SafeRelease(m_pixelShader);
}