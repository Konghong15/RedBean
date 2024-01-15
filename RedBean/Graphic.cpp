#include "pch.h"

#include "Graphic.h"

#include "RenderStates.h"
#include "VertexTypes.h"
#include "ShaderPrograms.h"
#include "ConstantBuffers.h"

namespace renderSystem
{
	bool Graphic::Init(HWND hwnd, HINSTANCE hInstance, UINT width, UINT height)
	{
		Release();

		mhWnd = hwnd;
		mhInstance = hInstance;
		mWidth = width;
		mHeight = height;

		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevel;
		HRESULT hr = D3D11CreateDevice(
			0,                 // default adapter
			md3dDriverType,
			0,                 // no software device
			createDeviceFlags,
			0, 0,              // default feature level array
			D3D11_SDK_VERSION,
			&md3dDevice,
			&featureLevel,
			&md3dContext);

		if (FAILED(hr))
		{
			MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
			return false;
		}

		if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
			return false;
		}

		md3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
		assert(m4xMsaaQuality > 0);

		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = mWidth;
		sd.BufferDesc.Height = mHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		if (mEnable4xMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m4xMsaaQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = mhWnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		IDXGIDevice* dxgiDevice = 0;
		md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

		IDXGIAdapter* dxgiAdapter = 0;
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

		IDXGIFactory* dxgiFactory = 0;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

		dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain);

		ReleaseCOM(dxgiDevice);
		ReleaseCOM(dxgiAdapter);
		ReleaseCOM(dxgiFactory);

		OnResize();

		builtIn::ShaderPrograms::InitAll(md3dDevice);
		builtIn::InputLayouts::InitAll(md3dDevice);
		builtIn::RenderStates::InitAll(md3dDevice);
		builtIn::ConstantBuffers::InitAll(md3dDevice);

		return true;
	}

	void Graphic::Release()
	{
		builtIn::ShaderPrograms::DestroyAll();
		builtIn::InputLayouts::DestroyAll();
		builtIn::RenderStates::DestroyAll();
		builtIn::ConstantBuffers::DestroyAll();

		ReleaseCOM(mRenderTargetView);
		ReleaseCOM(mDepthStencilView);
		ReleaseCOM(mSwapChain);
		ReleaseCOM(mDepthStencilBuffer);

		if (md3dContext)
			md3dContext->ClearState();

		ReleaseCOM(md3dContext);
		ReleaseCOM(md3dDevice);
	}

	void Graphic::BeginRender()
	{
		ID3D11ShaderResourceView* const SRVs[] = { NULL };
		md3dContext->PSSetShaderResources(0, 1, SRVs);

		md3dContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
		md3dContext->ClearRenderTargetView(mRenderTargetView, common::color::White);
		md3dContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// const size_t OFFSCREEN_SIZE = static_cast<size_t>(eDefferedTexture::Size);
		// ID3D11RenderTargetView* renderTargets[OFFSCREEN_SIZE];
		// 
		// assert(mRenderTargets.size() == OFFSCREEN_SIZE);
		// for (size_t i = 0; i < OFFSCREEN_SIZE; ++i)
		// {
		// 	renderTargets[i] = mRenderTargets[i].GetTextureRTV();
		// }
		// 
		// md3dContext->OMSetRenderTargets(OFFSCREEN_SIZE, renderTargets, mDepthStencilView);
		// 
		// // 오프스크린의 랜더타겟 뷰, 뎁스스탠실 뷰를 클리어한다.
		// for (size_t i = 0; i < OFFSCREEN_SIZE; ++i)
		// {
		// 	mRenderTargets[i].ClearRenderTarget(common::color::Black);
		// }
		// 
		// md3dContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void Graphic::EndRender()
	{
		// 조명 적용 후 디퍼드 데이터 합치기

		// 후처리하기

		// 후면버퍼에 그리기 + present
		mSwapChain->Present(0, 0);
	}

	void Graphic::OnResize()
	{
		assert(md3dContext);
		assert(md3dDevice);
		assert(mSwapChain);

		ReleaseCOM(mRenderTargetView);
		ReleaseCOM(mDepthStencilView); // 랜더링파이프라인에 텍스처를 연결하기 위한 인터페이스 클래스
		ReleaseCOM(mDepthStencilBuffer); // 텍스처

		HR(mSwapChain->ResizeBuffers(1, mWidth, mHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		ID3D11Texture2D* backBuffer = nullptr;
		HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
		HR(md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
		ReleaseCOM(backBuffer);

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = mWidth;
		depthStencilDesc.Height = mHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		if (mEnable4xMsaa)
		{
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
		}
		else
		{
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}

		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		HR(md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
		HR(md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));

		md3dContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

		mScreenViewport.TopLeftX = 0;
		mScreenViewport.TopLeftY = 0;
		mScreenViewport.Width = static_cast<float>(mWidth);
		mScreenViewport.Height = static_cast<float>(mHeight);
		mScreenViewport.MinDepth = 0.0f;
		mScreenViewport.MaxDepth = 1.0f;

		md3dContext->RSSetViewports(1, &mScreenViewport);
	}
}