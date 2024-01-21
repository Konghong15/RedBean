#include "pch.h"

#include "DepthStencil.h"
#include "RenderTarget.h"
#include "Graphics.h"

namespace Bind
{
	DepthStencil::DepthStencil(Graphics& graphics, UINT width, UINT height, bool canBindShaderInput)
	{
		// 깊이 스텐실 텍스처 생성
		ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | (canBindShaderInput ? D3D11_BIND_SHADER_RESOURCE : 0);
		(GetDevice(graphics)->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		// 깊이 스텐실 뷰 생성
		GetDevice(graphics)->CreateDepthStencilView(
			pDepthStencil.Get(),
			nullptr,
			&pDepthStencilView
		);
	}

	void DepthStencil::BindBuffer(Graphics& graphics)
	{
		GetContext(graphics)->OMSetRenderTargets(0, nullptr, pDepthStencilView.Get());
	}

	void DepthStencil::BindBuffer(Graphics& graphics, BufferResource* renderTarget)
	{
		assert(dynamic_cast<RenderTarget*>(renderTarget) != nullptr);
		BindBuffer(graphics, static_cast<RenderTarget*>(renderTarget));
	}

	void DepthStencil::BindBuffer(Graphics& graphics, RenderTarget* renderTarget)
	{
		renderTarget->BindBuffer(graphics, this);
	}

	void DepthStencil::Clear(Graphics& graphics)
	{
		GetContext(graphics)->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	}

	ShaderInputDepthStencil::ShaderInputDepthStencil(Graphics& graphics, UINT slot)
		: ShaderInputDepthStencil(graphics, graphics.GetWidth(), graphics.GetHeight(), slot)
	{
	}

	ShaderInputDepthStencil::ShaderInputDepthStencil(Graphics& graphics, UINT width, UINT height, UINT slot)
		: DepthStencil(graphics, width, height, true)
		, slot(slot)
	{
		ComPtr<ID3D11Resource> pRes;
		pDepthStencilView->GetResource(&pRes);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; 
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		
		GetDevice(graphics)->CreateShaderResourceView(
			pRes.Get(),
			&srvDesc,
			&pShaderResourceView
		);
	}

	void ShaderInputDepthStencil::Bind(Graphics& graphics)
	{
		GetContext(graphics)->PSSetShaderResources(slot, 1u, pShaderResourceView.GetAddressOf());
	}

	OutputOnlyDepthStencil::OutputOnlyDepthStencil(Graphics& graphics)
		: OutputOnlyDepthStencil(graphics, graphics.GetWidth(), graphics.GetHeight())
	{
	}

	OutputOnlyDepthStencil::OutputOnlyDepthStencil(Graphics& graphics, UINT width, UINT height)
		: DepthStencil(graphics, width, height, false)
	{
	}

	void OutputOnlyDepthStencil::Bind(Graphics& graphics)
	{
		assert("출력 전용은 입력으로 바인딩되면 안됩니다." && false);
	}
}
