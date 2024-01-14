#pragma once

#include "RenderTarget.h"

namespace renderSystem
{
	// 위치 및 일반 텍스처는 16 ~ 32비트, 색상 값은 8비트 정밀도로 처리할 예정
	enum class eDefferedTexture
	{
		Albedo,
		Normal,
		PositionH,
		Specular,
		Size
	};

	class Graphic
	{
	public:
		Graphic() = default;
		~Graphic() = default;
		Graphic(const Graphic&) = delete;
		Graphic& operator=(const Graphic&) = delete;

		bool Init(HWND hwnd, HINSTANCE hInstance, UINT width, UINT height);
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
		HINSTANCE mhInstance;
		HWND mhWnd;
		bool mbPaused;
		bool mbMinimized;
		bool mbMaximized;
		bool mbResizing;
		UINT m4xMsaaQuality;

		ID3D11Device* md3dDevice;
		ID3D11DeviceContext* md3dContext;
		IDXGISwapChain* mSwapChain;
		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		D3D11_VIEWPORT mScreenViewport;

		D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
		UINT mWidth;
		UINT mHeight;
		bool mEnable4xMsaa;


		// // 깊이버퍼는 하나만 쓰고 렌더타겟은 여러개 써야하니까 이 클래스에 SRV랑 RTV만 만드는 게 나을듯?
		// array<directXWrapper::RenderTarget, static_cast<size_t>(eDefferedTexture::Size)> mRenderTargets;
		// 
		// // 합칠때 쓰는 넘
		// ID3D11Buffer* mScreenQuadVB;
		// ID3D11Buffer* mScreenQuadIB;
	};
}