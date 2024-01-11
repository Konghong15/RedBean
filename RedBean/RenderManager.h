#pragma once

#include "RenderTarget.h"

class RenderObject;

// 위치 및 일반 텍스처는 16 ~ 32비트, 색상 값은 8비트 정밀도로 처리할 예정
enum class eDefferedTexture
{
	Albedo, 
	Normal, 
	PositionH,
	Specular, 
	Size
};

class RenderManager
{
public:
	RenderManager() = default;
	~RenderManager() = default;
	RenderManager(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager&) = delete;

	bool Init(HWND hwnd, HINSTANCE hInstance, UINT width, UINT height);
	void Release();

	void BeginRender();
	void Render();
	void EndRender();

	void OnResize();

	void AddRenderObject(RenderObject* renderObject) { mRenderObjects.push_back(renderObject); }

	HINSTANCE GetInstance() const { return mhInstance; }
	HWND GetHWnd() const { return mhWnd; }
	UINT GetWidth() const { return mWidth; }
	UINT GetHeight() const { return mHeight; }
	float GetAspectRatio() const {}
	common::Camera& GetCamera() { return mCamera; }
	const common::Camera& GetCamera() const { return mCamera; }

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

	D3D_DRIVER_TYPE md3dDriverType;
	UINT mWidth;
	UINT mHeight;
	bool mEnable4xMsaa;

	vector<RenderObject*> mRenderObjects;
	common::Camera mCamera;

	// 깊이버퍼는 하나만 쓰고 렌더테겟은 여러개 써야하니까 이 클래스에 SRV랑 RTV만 만드는 게 나을듯?
	array<directXWrapper::RenderTarget, static_cast<size_t>(eDefferedTexture::Size)> mRenderTargets;

	// 합칠때 쓰는 넘
	ID3D11Buffer* mScreenQuadVB;
	ID3D11Buffer* mScreenQuadIB;
};

