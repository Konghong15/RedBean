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
		// 참이면 플래그 추가
		UINT flag = depthClear ? D3D11_CLEAR_DEPTH : 0  
			| stencilClear ? D3D11_CLEAR_STENCIL : 0; 

		md3dContext->ClearDepthStencilView(mDepthStencilDSV.Get(), flag, depthValue, stencilValue);
	}

	void RenderTarget::createTexture()
	{
		HRESULT hr = S_OK;

		// 1.텍스처(2D) 속성 선언 및 초기화
		D3D11_TEXTURE2D_DESC TexDesc;
		ZeroMemory(&TexDesc, sizeof(TexDesc));

		// 2.속성 세팅
		TexDesc.Width = mWidth;						// 가로 크기(파라미터)
		TexDesc.Height = mHeight;					// 세로 크기(파라미터)
		TexDesc.MipLevels = 1;						// 밉맵의 최대 레벨			♣(알아보자 마크)
		TexDesc.ArraySize = 1;						// 텍스쳐 배열의 텍스쳐 수	♣(알아보자 마크)
		TexDesc.Format = mDXGIFormat;				// 형식(파라미터)
		TexDesc.SampleDesc.Count = mValueAA;			// AA(안티앨리어싱)
		TexDesc.Usage = D3D11_USAGE_DEFAULT;		// 텍스쳐 읽고 쓰는 방식(기본)
		TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// 용도(렌더 타겟과 쉐이더리소스 용도)
		TexDesc.CPUAccessFlags = 0;					// CPU의 접근은 허용하지 않는다.
		TexDesc.MiscFlags = 0;						// 리소스 옵션(아무것도 쓰지 않음)

		// 3.텍스쳐 생성
		ID3D11Texture2D* pTex = NULL;
		HR(md3dDevice->CreateTexture2D(&TexDesc, NULL, mTextureBuffer.GetAddressOf()));
	}
	void RenderTarget::createRenderTargetView()
	{
		// 1.렌더타겟뷰 속성 선언 및 초기화
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		ZeroMemory(&RTVDesc, sizeof(RTVDesc));

		// 2.속성 세팅
		RTVDesc.Format = mDXGIFormat;		// 형식(파라미터): 렌더타겟텍스처와 동일하게.
		RTVDesc.Texture2D.MipSlice = 0;			// 2D 렌더타겟용 추가 옵션: 밉맵 분할용 ♣(알아보자 마크)

		if (mValueAA == 1)
		{
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;		// 2D 렌더타겟
		}

		else
		{
			RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;	// 2D 렌더타겟 + AA
		}

		// 3.렌더타겟뷰 생성
		HR(md3dDevice->CreateRenderTargetView(mTextureBuffer.Get(), &RTVDesc, mTextureRTV.GetAddressOf()));
	}
	void RenderTarget::createShaderResourceView()
	{
		//셰이더리소스뷰 정보 구성.
		D3D11_SHADER_RESOURCE_VIEW_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Format = mDXGIFormat;							//텍스처와 동일포멧유지.
		sd.Texture2D.MipLevels = 1;								//2D SRV 추가 설정 : 밉멥 설정.
		sd.Texture2D.MostDetailedMip = 0;
		//sd.Texture2DMS.UnusedField_NothingToDefine = 0;		//2D SRV+AA 추가 설정

		if (mValueAA == 1)
		{
			sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;		//2D SRV.
		}

		else
		{
			sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;		//2D SRV + AA
		}

		//셰이더리소스뷰 생성.
		HRESULT hr = md3dDevice->CreateShaderResourceView(mTextureBuffer.Get(), &sd, mTextureSRV.GetAddressOf());
	}

	void RenderTarget::createDepthStencilView()
	{
		// 텍스쳐(버퍼) 생성
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = mWidth;
		td.Height = mHeight;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT; // 이렇게 큰 버퍼로 잡을 필요가 있나?

		td.SampleDesc.Count = mValueAA;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.MiscFlags = 0;

		HR(md3dDevice->CreateTexture2D(&td, NULL, mDepthStencilBuffer.GetAddressOf()));

		// 깊이/스텐실 뷰 생성
		D3D11_DEPTH_STENCIL_VIEW_DESC dd;
		ZeroMemory(&dd, sizeof(dd));
		dd.Format = td.Format;
		dd.Texture2D.MipSlice = 0;

		if (mValueAA == 1)
		{
			dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;			//2D (AA 없음)
		}

		else
		{
			dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;			//2D (AA 적용)
		}

		//깊이/스텐실 뷰 생성.
		md3dDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dd, mDepthStencilDSV.GetAddressOf());
	}
}