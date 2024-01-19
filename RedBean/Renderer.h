#pragma once

#include "RenderTarget.h"

namespace renderSystem
{
	// // ��ġ �� �Ϲ� �ؽ�ó�� 16 ~ 32��Ʈ, ���� ���� 8��Ʈ ���е��� ó���� ����
	// enum class eDefferedTexture
	// {
	// 	Albedo,
	// 	Normal,
	// 	PositionH,
	// 	Specular,
	// 	Size
	// };

	class Renderer
	{
	public:
		Renderer() = default;
		~Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		bool Init(HWND hwnd, HINSTANCE hInstance, UINT width, UINT height, bool bEnable4xMsaa = false);
		void Release();
		void OnResize();
		void BeginRender();
		void EndRender();

		HINSTANCE GetInstance() const { return mhInstance; }
		HWND GetHWnd() const { return mhWnd; }
		UINT GetWidth() const { return mWidth; }
		UINT GetHeight() const { return mHeight; }
		float GetAspectRatio() const { return (float)mWidth / mHeight; }

		ID3D11Device* GetDevice() { return md3dDevice; }
		ID3D11DeviceContext* GetContext() { return md3dContext; }

	private:
		HINSTANCE mhInstance = NULL;
		HWND mhWnd = NULL;

		ID3D11Device* md3dDevice = nullptr;
		ID3D11DeviceContext* md3dContext = nullptr;
		IDXGISwapChain* mSwapChain = nullptr;
		ID3D11Texture2D* mDepthStencilBuffer = nullptr;
		ID3D11RenderTargetView* mRenderTargetView = nullptr;
		ID3D11DepthStencilView* mDepthStencilView = nullptr;
		D3D11_VIEWPORT mScreenViewport = {};

		D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
		UINT mWidth = 0;
		UINT mHeight = 0;
		UINT m4xMsaaQuality = 0;
		bool mbEnable4xMsaa = false;

		// // ���̹��۴� �ϳ��� ���� ����Ÿ���� ������ ����ϴϱ� �� Ŭ������ SRV�� RTV�� ����� �� ������?
		// array<directXWrapper::RenderTarget, static_cast<size_t>(eDefferedTexture::Size)> mRenderTargets;
		// 
		// // ��ĥ�� ���� ��
		// ID3D11Buffer* mScreenQuadVB;
		// ID3D11Buffer* mScreenQuadIB;
	};
}