#pragma once

#include "IBindable.h"
#include "BufferResource.h"

class Graphics;

namespace Bind
{
	class DepthStencil;

	class RenderTarget : public IBindable, public BufferResource
	{
	public:
		void BindBuffer(Graphics& graphics) override;
		void BindBuffer(Graphics& graphics, BufferResource* depthStencil) override;
		void BindBuffer(Graphics& graphics, DepthStencil* depthStencil);
		void Clear(Graphics& graphics) override;
		void Clear(Graphics& graphics, const std::array<float, 4>& color);

		UINT GetWidth() const;
		UINT GetHeight() const;

	protected:
		RenderTarget(Graphics& graphics, ID3D11Texture2D* pTexture);
		RenderTarget(Graphics& graphics, UINT width, UINT height);

	private:
		void bindBuffer(Graphics& graphics, ID3D11DepthStencilView* pDepthStencilView);

	protected:
		UINT mWidth;
		UINT mHeight;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mpTargetView;
	};

	class ShaderInputRenderTarget : public RenderTarget
	{
	public:
		ShaderInputRenderTarget(Graphics& graphics, UINT width, UINT height, UINT slot);
		void Bind(Graphics& graphics) override;

	private:
		UINT mSlot;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mpShaderResourceView;
	};

	class OutputOnlyRenderTarget : public RenderTarget
	{
		friend Graphics;
	public:
		void Bind(Graphics& graphics) override;

	private:
		OutputOnlyRenderTarget(Graphics& graphics, ID3D11Texture2D* pTexture);
	};
}