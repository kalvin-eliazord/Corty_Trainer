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
	DXGI_SWAP_CHAIN_DESC sDesc{ 0 };
	sDesc.BufferCount = 1;
	sDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sDesc.OutputWindow = GetDesktopWindow();
	sDesc.Windowed = TRUE;
	sDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sDesc.SampleDesc.Count = 1;

	ID3D11Device* mDevice = nullptr;
	IDXGISwapChain* pSwapchain = nullptr;

	HRESULT hResult{ D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sDesc, &pSwapchain, &mDevice, nullptr, nullptr) };

	if (GetLastError() == 0x594) SetLastError(0); //ignore eror related to output window //TODO: fix this little issue with window handle

	if (FAILED(hResult)) return false;

	void** pVMT{ *reinterpret_cast<void***>(pSwapchain) };

	// Get Present's address out of VMT
	oPresent = static_cast<TPresent>(pVMT[static_cast<UINT>(IDXGISwapChainVMT::Present)]);

	//SafeRelease(pSwapchain);
	SafeRelease(mDevice);

	return true;
}

bool MyD3d11::CompileShader(const char* szShader, const char* szEntrypoint, const char* szTarget, ID3D10Blob** pCompiledShaderBlob)
{
	ID3D10Blob* pErrorBlob{ nullptr };

	auto hRes{ D3DCompile(szShader, strlen(szShader), 0, nullptr, nullptr, szEntrypoint, szTarget, D3DCOMPILE_ENABLE_STRICTNESS, 0, pCompiledShaderBlob, &pErrorBlob) };
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
	ID3D10Blob* pCompiledShaderBlob{ nullptr };

	// create vertex shader
	if (!CompileShader(shaders, "VS", "vs_5_0", &pCompiledShaderBlob))
		return false;

	HRESULT hRes{ mDevice->CreateVertexShader(pCompiledShaderBlob->GetBufferPointer(), pCompiledShaderBlob->GetBufferSize(), nullptr, &mVertexShader) };
	if (FAILED(hRes)) return false;

	// Define/create the input layout for the vertex shader
	D3D11_INPUT_ELEMENT_DESC layout[2] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements{ ARRAYSIZE(layout) };

	hRes = mDevice->CreateInputLayout(layout, numElements, pCompiledShaderBlob->GetBufferPointer(), pCompiledShaderBlob->GetBufferSize(), &mVertexLayout);
	if (FAILED(hRes)) return false;

	mContext->IASetInputLayout(mVertexLayout);

	SafeRelease(pCompiledShaderBlob);

	if (!CompileShader(shaders, "PS", "ps_5_0", &pCompiledShaderBlob))
		return false;

	hRes = mDevice->CreatePixelShader(pCompiledShaderBlob->GetBufferPointer(), pCompiledShaderBlob->GetBufferSize(), nullptr, &mPixelShader);
	if (FAILED(hRes)) return false;

	return true;
}

bool MyD3d11::GetViewport()
{
	UINT numViewports{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };

	// Apparently this isn't universal. Works on most games
	mContext->RSGetViewports(&numViewports, mViewports);

	//if it doesn't work, make your own
	if (!numViewports || !mViewports[0].Width)
	{
		mHwnd = HWindow::GetProcessWindow();

		if (!GetClientRect(mHwnd, &mHwndRect))
			return false;

		// Setup viewport
		myViewport.Width = static_cast<FLOAT>(mHwndRect.right);
		myViewport.Height = static_cast<FLOAT>(mHwndRect.bottom);
		myViewport.TopLeftX = static_cast<FLOAT>(mHwndRect.left);
		myViewport.TopLeftY = static_cast<FLOAT>(mHwndRect.top);
		myViewport.MinDepth = 0.0f;
		myViewport.MaxDepth = 1.0f;
		mContext->RSSetViewports(1, &myViewport);
	}
	else
	{
		myViewport = mViewports[0];
		mContext->RSSetViewports(1, &myViewport);
	}
	return true;
}

bool MyD3d11::SetupOrtho()
{
	mOrtho = DirectX::XMMatrixOrthographicOffCenterLH(0, myViewport.Width, myViewport.Height, 0, 0.0f, 1.0f);

	D3D11_BUFFER_DESC buffer_desc{ 0 };
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.ByteWidth = sizeof(mOrtho);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subresource_line{ 0 };
	subresource_line.pSysMem = &mOrtho;
	HRESULT hRes = mDevice->CreateBuffer(&buffer_desc, &subresource_line, &mConstantBuffer);
	if (FAILED(hRes))
		return false;

	mContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);

	return true;
}

bool MyD3d11::GetDeviceContextRenderTarget(IDXGISwapChain* pSwapchain)
{
	// Get game device
	HRESULT hRes{ pSwapchain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&mDevice)) };
	if (FAILED(hRes)) return false;

	mDevice->GetImmediateContext(&mContext);
	mContext->OMGetRenderTargets(1, &mRenderTargetView, nullptr);

	// mRenderTargetView backup
	if (!mRenderTargetView)
	{
		ID3D11Texture2D* backbuffer{ nullptr };
		hRes = pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer));
		if (FAILED(hRes)) return false;

		hRes = mDevice->CreateRenderTargetView(backbuffer, nullptr, &mRenderTargetView);
		backbuffer->Release();
		if (FAILED(hRes)) return false;

		mContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);
	}

	return true;
}

bool MyD3d11::InitD3DDraw(IDXGISwapChain* pSwapchain)
{
	mSwapChain = pSwapchain;

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
	{ DirectX::XMFLOAT3(x, y, 1.0f), color },
	{ DirectX::XMFLOAT3(x2, y2, 1.0f),	color },
	};

	D3D11_BUFFER_DESC buffer_desc_line{ 0 };
	buffer_desc_line.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc_line.ByteWidth = sizeof(Vertex) * 2;
	buffer_desc_line.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subresource_line{ 0 };
	subresource_line.pSysMem = &pVerts;

	mDevice->CreateBuffer(&buffer_desc_line, &subresource_line, &mVertexBuffer);

	mContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);

	UINT stride{ sizeof(Vertex) };
	UINT offset{ 0 };

	mContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mContext->IASetInputLayout(mVertexLayout);
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	mContext->VSSetShader(mVertexShader, nullptr, 0);
	mContext->PSSetShader(mPixelShader, nullptr, 0);

	mContext->RSSetViewports(1, &myViewport); // already set in the game's context

	mContext->Draw(2, 0);
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

	mDevice->CreateBuffer(&buffer_desc_line, &subresource_line, &mVertexBuffer);

	mContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);// we do this in init

	// Make sure the input assembler knows how to process our verts/indices
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	mContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mContext->IASetInputLayout(mVertexLayout);
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Set the shaders we need to render our line
	mContext->VSSetShader(mVertexShader, nullptr, 0);
	mContext->PSSetShader(mPixelShader, nullptr, 0);

	// Set viewport to context, unnecessary so far
	//mContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	mContext->RSSetViewports(1, &myViewport);

	mContext->Draw(2, 0);
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

	mDevice->CreateBuffer(&buffer_desc_line, &subresource_line, &mVertexBuffer);

	mContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);

	// Make sure the input assembler knows how to process our verts/indices
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	mContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mContext->IASetInputLayout(mVertexLayout);
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// Set the shaders we need to render our line
	mContext->VSSetShader(mVertexShader, nullptr, 0);
	mContext->PSSetShader(mPixelShader, nullptr, 0);

	// Set viewport to context, unnecessary so far?
	//mContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	mContext->RSSetViewports(1, &myViewport);

	mContext->Draw(5, 0);
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
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
	SafeRelease(mConstantBuffer);
	SafeRelease(mPixelShader);
	SafeRelease(mVertexShader);
	SafeRelease(mVertexLayout);
}
