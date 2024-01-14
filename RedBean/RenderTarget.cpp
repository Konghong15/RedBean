#include "pch.h"

#include "RenderTarget.h"

namespace directXWrapper
{
	bool RenderTarget::Init(ID3D11Device* device, ID3D11DeviceContext* context, const Desc& desc)
	{
		md3dDevice = device;
		md3dContext = context;

		mWidth = desc.Width;
		mWidth = desc.Height;

		switch (desc.Type)
		{
		case eRenderTargetType::R32_MAP:
			mDXGIFormat = DXGI_FORMAT_R32_FLOAT;
			break;
		case eRenderTargetType::RGBA32_MAP:
			mDXGIFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		default:
			assert(false);
		}

		createTexture();
		createRenderTargetView();
		createShaderResourceView();
		createDepthStencilView();
	
		return true;
	}
	bool RenderTarget::Destroy()
	{
		mTextureBuffer.Reset();
		mTextureRTV.Reset();
		mTextureSRV.Reset();
		mDepthStencilBuffer.Reset();
		mDepthStencilDSV.Reset();

		return true;
	}

	void RenderTarget::Resize(const Desc& desc)
	{
		// D3D11 Resource Reset
		Destroy();

		mWidth = desc.Width;
		mHeight = desc.Height;
		mValueAA = desc.ValueAA;

		createTexture();
		createRenderTargetView();
		createShaderResourceView();
		createDepthStencilView();
	}

	void RenderTarget::BindRenderTarget()
	{
		md3dContext->OMSetRenderTargets(1, mTextureRTV.GetAddressOf(), mDepthStencilDSV.Get());
	}

	void RenderTarget::ClearRenderTarget(const DirectX::SimpleMath::Color& color)
	{
		md3dContext->ClearRenderTargetView(mTextureRTV.Get(), color);
		ClearDepthStencil();
	}

	void RenderTarget::ClearRenderTarget(float r, float g, float b, float a)
	{
		ClearRenderTarget({ r,g,b,a });
	}
	void RenderTarget::ClearDepthStencil(bool depthClear, float depthValue, bool stencilClear, unsigned char stencilValue)
	{
		// ���̸� �÷��� �߰�
		UINT flag = depthClear ? D3D11_CLEAR_DEPTH : 0  
			| stencilClear ? D3D11_CLEAR_STENCIL : 0; 

		md3dContext->ClearDepthStencilView(mDepthStencilDSV.Get(), flag, depthValue, stencilValue);
	}

	void RenderTarget::createTexture()
	{
		HRESULT hr = S_OK;

		// 1.�ؽ�ó(2D) �Ӽ� ���� �� �ʱ�ȭ
		D3D11_TEXTURE2D_DESC TexDesc;
		ZeroMemory(&TexDesc, sizeof(TexDesc));

		// 2.�Ӽ� ����
		TexDesc.Width = mWidth;						// ���� ũ��(�Ķ����)
		TexDesc.Height = mHeight;					// ���� ũ��(�Ķ����)
		TexDesc.MipLevels = 1;						// �Ӹ��� �ִ� ����			��(�˾ƺ��� ��ũ)
		TexDesc.ArraySize = 1;						// �ؽ��� �迭�� �ؽ��� ��	��(�˾ƺ��� ��ũ)
		TexDesc.Format = mDXGIFormat;				// ����(�Ķ����)
		TexDesc.SampleDesc.Count = mValueAA;			// AA(��Ƽ�ٸ����)
		TexDesc.Usage = D3D11_USAGE_DEFAULT;		// �ؽ��� �а� ���� ���(�⺻)
		TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// �뵵(���� Ÿ�ٰ� ���̴����ҽ� �뵵)
		TexDesc.CPUAccessFlags = 0;					// CPU�� ������ ������� �ʴ´�.
		TexDesc.MiscFlags = 0;						// ���ҽ� �ɼ�(�ƹ��͵� ���� ����)

		// 3.�ؽ��� ����
		ID3D11Texture2D* pTex = NULL;
		HR(md3dDevice->CreateTexture2D(&TexDesc, NULL, mTextureBuffer.GetAddressOf()));
	}
	void RenderTarget::createRenderTargetView()
	{
		// 1.����Ÿ�ٺ� �Ӽ� ���� �� �ʱ�ȭ
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		ZeroMemory(&RTVDesc, sizeof(RTVDesc));

		// 2.�Ӽ� ����
		RTVDesc.Format = mDXGIFormat;		// ����(�Ķ����): ����Ÿ���ؽ�ó�� �����ϰ�.
		RTVDesc.Texture2D.MipSlice = 0;			// 2D ����Ÿ�ٿ� �߰� �ɼ�: �Ӹ� ���ҿ� ��(�˾ƺ��� ��ũ)

		if (mValueAA == 1)
		{
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;		// 2D ����Ÿ��
		}

		else
		{
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;	// 2D ����Ÿ�� + AA
		}

		// 3.����Ÿ�ٺ� ����
		HR(md3dDevice->CreateRenderTargetView(mTextureBuffer.Get(), &RTVDesc, mTextureRTV.GetAddressOf()));
	}
	void RenderTarget::createShaderResourceView()
	{
		//���̴����ҽ��� ���� ����.
		D3D11_SHADER_RESOURCE_VIEW_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Format = mDXGIFormat;							//�ؽ�ó�� ������������.
		sd.Texture2D.MipLevels = 1;								//2D SRV �߰� ���� : �Ӹ� ����.
		sd.Texture2D.MostDetailedMip = 0;
		//sd.Texture2DMS.UnusedField_NothingToDefine = 0;		//2D SRV+AA �߰� ����

		if (mValueAA == 1)
		{
			sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;		//2D SRV.
		}

		else
		{
			sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;		//2D SRV + AA
		}

		//���̴����ҽ��� ����.
		HRESULT hr = md3dDevice->CreateShaderResourceView(mTextureBuffer.Get(), &sd, mTextureSRV.GetAddressOf());
	}

	void RenderTarget::createDepthStencilView()
	{
		// �ؽ���(����) ����
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = mWidth;
		td.Height = mHeight;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT; // �̷��� ū ���۷� ���� �ʿ䰡 �ֳ�?

		td.SampleDesc.Count = mValueAA;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.MiscFlags = 0;

		HR(md3dDevice->CreateTexture2D(&td, NULL, mDepthStencilBuffer.GetAddressOf()));

		// ����/���ٽ� �� ����
		D3D11_DEPTH_STENCIL_VIEW_DESC dd;
		ZeroMemory(&dd, sizeof(dd));
		dd.Format = td.Format;
		dd.Texture2D.MipSlice = 0;

		if (mValueAA == 1)
		{
			dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;			//2D (AA ����)
		}

		else
		{
			dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;			//2D (AA ����)
		}

		//����/���ٽ� �� ����.
		md3dDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dd, mDepthStencilDSV.GetAddressOf());
	}
}