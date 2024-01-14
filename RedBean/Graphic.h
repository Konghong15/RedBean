#pragma once

#include "RenderTarget.h"

namespace renderSystem
{
	// ��ġ �� �Ϲ� �ؽ�ó�� 16 ~ 32��Ʈ, ���� ���� 8��Ʈ ���е��� ó���� ����
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


		// // ���̹��۴� �ϳ��� ���� ����Ÿ���� ������ ����ϴϱ� �� Ŭ������ SRV�� RTV�� ����� �� ������?
		// array<directXWrapper::RenderTarget, static_cast<size_t>(eDefferedTexture::Size)> mRenderTargets;
		// 
		// // ��ĥ�� ���� ��
		// ID3D11Buffer* mScreenQuadVB;
		// ID3D11Buffer* mScreenQuadIB;
	};
}