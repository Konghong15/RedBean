#include "pch.h"

#include "Graphics.h"
#include "RenderTarget.h"

Graphics::Graphics(HWND hWnd, int width, int height)
	: mWidth(width)
	, mHeight(height)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&mpSwapChain,
		&mpDevice,
		nullptr,
		&mpContext
	);

	// 후면버퍼를 참조로하는 RenderTarget 생성
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer);
	mpRenderTarget = std::shared_ptr<Bind::RenderTarget>{ new Bind::OutputOnlyRenderTarget(*this, pBackBuffer.Get()) };

	// 뷰포트 설정
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	mpContext->RSSetViewports(1u, &vp);
}

void Graphics::BeginFrame()
{
}
void Graphics::DrawIndexed(UINT count)
{
	mpContext->DrawIndexed(count, 0u, 0u);
}
void Graphics::EndFrame()
{
	mpSwapChain->Present(1u, 0u);
}

void Graphics::SetProjection(Matrix proj)
{
	mProjection = proj;
}
void Graphics::SetView(Matrix view)
{
	mView = view;
}
//void Graphics::SetIsEnableImgui(bool bIsEnableImgui)
//{
//
//}

//bool Graphics::IsEnableImgui()
//{
//	
//}
