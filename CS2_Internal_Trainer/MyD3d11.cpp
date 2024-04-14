#include "MyD3d11.h"

void MyD3d11::SafeRelease(auto* pData)
{
	if (pData)
	{
		pData->Release();
		pData = nullptr;
	}

}

bool MyD3d11::Set_oPresent()
{
	DXGI_SWAP_CHAIN_DESC sd{ 0 };
	sd.BufferCount = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.OutputWindow = GetDesktopWindow();
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.SampleDesc.Count = 1;

	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapchain = nullptr;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &pSwapchain, &pDevice, nullptr, nullptr);

	if (GetLastError() == 0x594) SetLastError(0); //ignore eror related to output window //TODO: fix this little issue with window handle

	if (FAILED(hr)) return false;

	void** pVMT = *reinterpret_cast<void***>(pSwapchain);

	// Get Present's address out of VMT
	oPresent = static_cast<TPresent>(pVMT[static_cast<UINT>(IDXGISwapChainVMT::Present)]);

	//SafeRelease(pSwapchain);
	SafeRelease(pDevice);

	return true;
}

bool MyD3d11::CompileShader(const char* szShader, const char* szEntrypoint, const char* szTarget, ID3D10Blob** pCompiledShaderBlob)
{
	ID3D10Blob* pErrorBlob = nullptr;

	auto hr = D3DCompile(szShader, strlen(szShader), 0, nullptr, nullptr, szEntrypoint, szTarget, D3DCOMPILE_ENABLE_STRICTNESS, 0, pCompiledShaderBlob, &pErrorBlob);
	if (FAILED(hr))
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
	ID3D10Blob* pCompiledShaderBlob = nullptr;

	// create vertex shader
	if (!CompileShader(shaders, "VS", "vs_5_0", &pCompiledShaderBlob))
		return false;

	HRESULT hr = pDevice->CreateVertexShader(pCompiledShaderBlob->GetBufferPointer(), pCompiledShaderBlob->GetBufferSize(), nullptr, &pVertexShader);
	if (FAILED(hr))
		return false;

	// Define/create the input layout for the vertex shader
	D3D11_INPUT_ELEMENT_DESC layout[2] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements = ARRAYSIZE(layout);

	hr = pDevice->CreateInputLayout(layout, numElements, pCompiledShaderBlob->GetBufferPointer(), pCompiledShaderBlob->GetBufferSize(), &pVertexLayout);
	if (FAILED(hr))
		return false;

	pContext->IASetInputLayout(pVertexLayout); //unnecesary for whatever reason, call it anyways

	SafeRelease(pCompiledShaderBlob);

	// create pixel shader
	if (!CompileShader(shaders, "PS", "ps_5_0", &pCompiledShaderBlob))
		return false;

	hr = pDevice->CreatePixelShader(pCompiledShaderBlob->GetBufferPointer(), pCompiledShaderBlob->GetBufferSize(), nullptr, &pPixelShader);
	if (FAILED(hr))
		return false;

	return true;
}

bool MyD3d11::GetViewport()
{
	UINT numViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;

	// Apparently this isn't universal. Works on most games
	pContext->RSGetViewports(&numViewports, pViewports);

	//if it doesn't work, make your own
	if (!numViewports || !pViewports[0].Width)
	{
		hWnd = HWindow::GetProcessWindow();

		if (!GetClientRect(hWnd, &windowRect))
			return false;

		// Setup viewport
		myViewport.Width = static_cast<FLOAT>(windowRect.right);
		myViewport.Height = static_cast<FLOAT>(windowRect.bottom);
		myViewport.TopLeftX = static_cast<FLOAT>(windowRect.left);
		myViewport.TopLeftY = static_cast<FLOAT>(windowRect.top);
		myViewport.MinDepth = 0.0f;
		myViewport.MaxDepth = 1.0f;
		pContext->RSSetViewports(1, &myViewport);
	}
	else
	{
		myViewport = pViewports[0];
		pContext->RSSetViewports(1, &myViewport);
	}
	return true;
}

bool MyD3d11::SetupOrtho()
{
	mOrtho = DirectX::XMMatrixOrthographicOffCenterLH(0, myViewport.Width, myViewport.Height, 0, 0.0f, 1.0f);

	// Create the constant buffer, this includes our ortho matrix which the vertex shader uses
	D3D11_BUFFER_DESC buffer_desc{ 0 };
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.ByteWidth = sizeof(mOrtho);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subresource_line{ 0 };
	subresource_line.pSysMem = &mOrtho;
	HRESULT hr = pDevice->CreateBuffer(&buffer_desc, &subresource_line, &pConstantBuffer);
	if (FAILED(hr))
		return false;

	pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

	return true;
}

bool MyD3d11::GetDeviceContextRenderTarget(IDXGISwapChain* pSwapchain)
{
	//Get the device, context and render target
	HRESULT hr = pSwapchain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&pDevice));
	if (FAILED(hr))
		return false;

	pDevice->GetImmediateContext(&pContext);
	pContext->OMGetRenderTargets(1, &pRenderTargetView, nullptr);

	// If for some reason we fail to get a render target, create one.
	if (!pRenderTargetView)
	{
		// Get a pointer to the back buffer for the render target view
		ID3D11Texture2D* pBackbuffer = nullptr;
		hr = pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackbuffer));
		if (FAILED(hr))
			return false;

		// Create render target view
		hr = pDevice->CreateRenderTargetView(pBackbuffer, nullptr, &pRenderTargetView);
		pBackbuffer->Release();
		if (FAILED(hr))
			return false;

		// Make sure our render target is set, only needed if creating our own, if it already exists just use the original
		pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
	}
	return true;
}

bool MyD3d11::InitD3DDraw(IDXGISwapChain* pSwapchain)
{
	this->pSwapchain = pSwapchain;

	if (!GetDeviceContextRenderTarget(pSwapchain)) return false;

	if (!CompileShaders()) return false;

	if (!GetViewport()) return false;

	if (!SetupOrtho()) return false;

	return true;
}

void MyD3d11::BeginDraw()
{
	GetViewport();
	SetupOrtho();
}

void MyD3d11::DrawLine(float x, float y, float x2, float y2, D3DCOLORVALUE color)
{
	// Setup vertices
	Vertex pVerts[2] = {
	{ DirectX::XMFLOAT3(x, y, 1.0f),		color },
	{ DirectX::XMFLOAT3(x2, y2, 1.0f),	color },
	};

	D3D11_BUFFER_DESC buffer_desc_line{ 0 };
	buffer_desc_line.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc_line.ByteWidth = sizeof(Vertex) * 2;
	buffer_desc_line.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subresource_line{ 0 };
	subresource_line.pSysMem = &pVerts;

	pDevice->CreateBuffer(&buffer_desc_line, &subresource_line, &pVertexBuffer);

	pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);// we do this in init

	// Make sure the input assembler knows how to process our verts/indices
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pContext->IASetInputLayout(pVertexLayout);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Set the shaders we need to render our line
	pContext->VSSetShader(pVertexShader, nullptr, 0);
	pContext->PSSetShader(pPixelShader, nullptr, 0);

	// Set viewport to context, unnecessary as we're using the games
	//pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	pContext->RSSetViewports(1, &myViewport); //not required, it's already set in the game's context

	//In all cases, Width and Height must be positive

	pContext->Draw(2, 0);
}

void MyD3d11::DrawLineWH(float x, float y, float width, float height, D3DCOLORVALUE color)
{
	// Setup vertices
	Vertex pVerts[2] = {
	{ DirectX::XMFLOAT3(x,			 y,			 1.0f),	color },
	{ DirectX::XMFLOAT3(x + width,   y + height, 1.0f),	color },
	};

	D3D11_BUFFER_DESC buffer_desc_line{ 0 };
	buffer_desc_line.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc_line.ByteWidth = sizeof(Vertex) * 2;
	buffer_desc_line.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subresource_line{ 0 };
	subresource_line.pSysMem = &pVerts;

	pDevice->CreateBuffer(&buffer_desc_line, &subresource_line, &pVertexBuffer);

	pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);// we do this in init

	// Make sure the input assembler knows how to process our verts/indices
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pContext->IASetInputLayout(pVertexLayout);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Set the shaders we need to render our line
	pContext->VSSetShader(pVertexShader, nullptr, 0);
	pContext->PSSetShader(pPixelShader, nullptr, 0);

	// Set viewport to context, unnecessary so far
	//pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	pContext->RSSetViewports(1, &myViewport);

	pContext->Draw(2, 0);
}

void MyD3d11::DrawBox(float x, float y, float width, float height, D3DCOLORVALUE color)
{
	// Setup vertices TL, TR, BR, BL

	Vertex pVerts[5] = {
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

	D3D11_SUBRESOURCE_DATA subresource_line{ 0 };
	subresource_line.pSysMem = &pVerts;

	pDevice->CreateBuffer(&buffer_desc_line, &subresource_line, &pVertexBuffer);

	pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

	// Make sure the input assembler knows how to process our verts/indices
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pContext->IASetInputLayout(pVertexLayout);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// Set the shaders we need to render our line
	pContext->VSSetShader(pVertexShader, nullptr, 0);
	pContext->PSSetShader(pPixelShader, nullptr, 0);

	// Set viewport to context, unnecessary so far?
	//pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	pContext->RSSetViewports(1, &myViewport);

	pContext->Draw(5, 0);
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
	SafeRelease(pVertexBuffer);
	SafeRelease(pIndexBuffer);
	SafeRelease(pConstantBuffer);
	SafeRelease(pPixelShader);
	SafeRelease(pVertexShader);
	SafeRelease(pVertexLayout);
}
