#pragma once

namespace Bind
{
	class RenderTarget;
}

class Graphics
{
	friend class GraphicsResource;

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

	inline Matrix GetProjection() const;
	inline Matrix GetView() const;
	//bool IsEnableImgui();
	inline UINT GetWidth() const;
	inline UINT GetHeight() const;
	inline std::shared_ptr<Bind::RenderTarget> GetRenderTarget();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> mpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mpContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mpSwapChain;
	std::shared_ptr<Bind::RenderTarget> mpRenderTarget;

	Matrix mProjection;
	Matrix mView;

	UINT mWidth;
	UINT mHeight;
};

Matrix Graphics::GetProjection() const
{
	return mProjection;
}
Matrix Graphics::GetView() const
{
	return mView;
}
UINT Graphics::GetWidth() const
{
	return mWidth;
}
UINT Graphics::GetHeight() const
{
	return mHeight;
}

std::shared_ptr<Bind::RenderTarget> Graphics::GetRenderTarget()
{
	return mpRenderTarget;
}
