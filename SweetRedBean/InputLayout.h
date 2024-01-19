#pragma once

#include "IBindable.h"
#include "Vertex.h"
#include "BindableResourceManager.h"

namespace bind
{
	class InputLayout : public IBindable
	{
	public:
		InputLayout(Graphics& graphics, dynamic::VertexLayout layout, ID3DBlob* pVSBytecode)
			:mLayout(std::move(layout))
		{
			const auto d3dLayout = layout.GetD3DLayout();

			GetDevice(graphics)->CreateInputLayout(
				d3dLayout.data(),
				(UINT)d3dLayout.size(),
				pVSBytecode->GetBufferPointer(),
				pVSBytecode->GetBufferSize(),
				&mpInputLayout
			);
		}
		~InputLayout() = default;

		static shared_ptr<InputLayout> Resolve(Graphics& graphics, const dynamic::VertexLayout& layout, ID3DBlob  pVSBytecode)
		{
			return ResourceManager::Create<InputLayout>(graphics, layout, pVSBytecode);
		}
		static string GenerateUID(const dynamic::VertexLayout& layout, ID3DBlob* pVSBytecode = nullptr)
		{
			return typeid(InputLayout).name() + "#"s + layout.GetCode();
		}

		virtual void Bind(Graphics& graphics) override
		{
			GetContext(graphics)->IASetInputLayout(mpInputLayout.Get());
		}

		const dynamic::VertexLayout GetLayout() const
		{
			return mLayout;
		}
		std::string GetUID() const noexcept override
		{
			return GenerateUID(mLayout);
		}

	protected:
		dynamic::VertexLayout mLayout;
		ComPtr<ID3D11InputLayout> mpInputLayout;
	};
}