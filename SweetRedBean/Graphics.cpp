#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hWnd, int width, int height)
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

	ComPtr<ID3D11Resource> pBackBuffer;
	hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	hr = mpDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &mpRTV);

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ComPtr<ID3D11DepthStencilState> pDSState;
	hr = mpDevice->CreateDepthStencilState(&dsDesc, &pDSState);

	mpContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = mpDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);

	hr = mpDevice->CreateDepthStencilView(pDepthStencil.Get(), NULL, mpDSV.GetAddressOf());

	// bind depth stensil view to OM
	mpContext->OMSetRenderTargets(1u, mpRTV.GetAddressOf(), mpDSV.Get());

	// configure viewport
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
	const float color[] = { 0.f, 0.f, 0.f, 1.0f };
	mpContext->ClearRenderTargetView(mpRTV.Get(), color);
	mpContext->ClearDepthStencilView(mpDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
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

Matrix Graphics::GetProjection() const
{
	return mProjection;
}
Matrix Graphics::GetView() const
{
	return mView;
}
//bool Graphics::IsEnableImgui()
//{
//	
//}
