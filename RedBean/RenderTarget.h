#pragma once

namespace directXWrapper
{
	enum class eRenderTargetType
	{
		RGBA32_MAP,
		R32_MAP
	};

	class RenderTarget
	{
		struct Desc
		{
			unsigned int Width = 512;
			unsigned int Height = 512;
			eRenderTargetType Type = eRenderTargetType::RGBA32_MAP;
			unsigned int ValueAA = 1;
		};

	public:
		RenderTarget() = default;
		~RenderTarget() = default;
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;

		bool Init(ID3D11Device* device, ID3D11DeviceContext* context, const Desc& desc);
		bool Destroy();

		void Resize(const Desc& desc);

		void BindRenderTarget(bool bIsClear = false, float r, float g, float b, float a);
		void ClearRenderTarget(const DirectX::SimpleMath::Color& color);
		void ClearRenderTarget(float r, float g, float b, float a);
		void ClearDepthStencil(bool depthClear = true, float depthValue = 1.0f, bool stencilClear = true, unsigned char stencilValue = 0);

		ID3D11Texture2D* GetTexutreBuffer() { return mTextureBuffer.Get(); }
		ID3D11RenderTargetView* GetTextureRTV() { return mTextureRTV.Get(); }
		ID3D11ShaderResourceView* GetTextureSRV() { return mTextureSRV.Get(); }

		ID3D11Texture2D* GetDepthStencilBuffer() { return mDepthStencilBuffer.Get(); }
		ID3D11DepthStencilView* GetDepthStencilDSV() { return mDepthStencilDSV.Get(); }

	private:
		void createTexture();
		void createRenderTargetView();
		void createShaderResourceView();
		void createDepthStencilView();

	private:
		ID3D11Device* md3dDevice;
		ID3D11DeviceContext* md3dContext;

		ComPtr<ID3D11Texture2D> mTextureBuffer;
		ComPtr<ID3D11RenderTargetView> mTextureRTV;
		ComPtr<ID3D11ShaderResourceView> mTextureSRV;

		ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> mDepthStencilDSV;

		DXGI_FORMAT mDXGIFormat;

		eRenderTargetType mType;
		unsigned int mWidth;
		unsigned int mHeight;
		unsigned int mValueAA;
	};
}