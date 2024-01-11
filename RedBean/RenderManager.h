#pragma once

#include "RenderTarget.h"

class RenderObject;

// ��ġ �� �Ϲ� �ؽ�ó�� 16 ~ 32��Ʈ, ���� ���� 8��Ʈ ���е��� ó���� ����
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

	// ���̹��۴� �ϳ��� ���� �����װ��� ������ ����ϴϱ� �� Ŭ������ SRV�� RTV�� ����� �� ������?
	array<directXWrapper::RenderTarget, static_cast<size_t>(eDefferedTexture::Size)> mRenderTargets;

	// ��ĥ�� ���� ��
	ID3D11Buffer* mScreenQuadVB;
	ID3D11Buffer* mScreenQuadIB;
};

