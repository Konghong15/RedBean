#pragma once
#include "IBindable.h"
#include "BufferResource.h"

class Graphics;

namespace Bind
{
	class RenderTarget;

	class DepthStencil : public IBindable, public BufferResource
	{
		friend RenderTarget;

	public:
		void BindBuffer(Graphics& graphics) override;
		void BindBuffer(Graphics& graphics, BufferResource* renderTarget)  override;
		void BindBuffer(Graphics& graphics, RenderTarget* renderTarget);
		void Clear(Graphics& graphics) override;

	protected:
		DepthStencil(Graphics& graphics, UINT width, UINT height, bool canBindShaderInput);
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	};

	// �Է�(SRV)���ε� ����� ���� ���ٽ� ����
	class ShaderInputDepthStencil : public DepthStencil
	{
	public:
		ShaderInputDepthStencil(Graphics& graphics, UINT slot);
		ShaderInputDepthStencil(Graphics& graphics, UINT width, UINT height, UINT slot);
		void Bind(Graphics& graphics)   override;
	private:
		UINT slot;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
	};

	// ��� ���� ���� ���ٽ� ����
	class OutputOnlyDepthStencil : public DepthStencil
	{
	public:
		OutputOnlyDepthStencil(Graphics& graphics);
		OutputOnlyDepthStencil(Graphics& graphics, UINT width, UINT height);
		void Bind(Graphics& graphics) override;
	};
}
