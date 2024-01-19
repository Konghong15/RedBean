#pragma once

class Graphics
{
	friend class IBindable;

public:
	Graphics(HWND hWnd, int width, int height);
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void BeginFrame();
	void DrawIndexed(UINT count);
	void EndFrame();

	void SetProjection(Matrix proj);
	void SetView(Matrix view);
	//void SetIsEnableImgui(bool bIsEnableImgui);

	Matrix GetProjection() const;
	Matrix GetView() const;
	//bool IsEnableImgui();

private:
	ComPtr<ID3D11Device> mpDevice;
	ComPtr<ID3D11DeviceContext> mpContext;
	ComPtr<IDXGISwapChain> mpSwapChain;
	ComPtr<ID3D11RenderTargetView> mpRTV;
	ComPtr<ID3D11DepthStencilView> mpDSV;

	Matrix mProjection;
	Matrix mView;
};

