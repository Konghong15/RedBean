#pragma once

#include "Graphics.h"
#include "IBindable.h"
#include "IDrawable.h"
#include "BindableResourceManager.h"

namespace bind
{
	template<typename C>
	class ConstantBuffer : public IBindable
	{
	public:
		ConstantBuffer(Graphics& graphics, const C& consts, UINT slot = 0u)
			: mSlot(slot)
		{
			D3D11_BUFFER_DESC cbd;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.ByteWidth = sizeof(consts);
			cbd.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA csd = {};
			csd.pSysMem = &consts;
			GetDevice(graphics)->CreateBuffer(&cbd, &csd, &mpConstantBuffer);
		}
		ConstantBuffer(Graphics& graphics, UINT slot = 0u)
			: mSlot(slot)
		{
			D3D11_BUFFER_DESC cbd;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.ByteWidth = sizeof(consts);
			cbd.StructureByteStride = 0u;

			GetDevice(graphics)->CreateBuffer(&cbd, NULL, &mpConstantBuffer);
		}

		void Update(Graphics& graphics, const C& consts)
		{
			D3D11_MAPPED_SUBRESOURCE msr;

			GetContext(graphics)->Map(
				mpConstantBuffer.Get(), 0u,
				D3D11_MAP_WRITE_DISCARD, 0u,
				&msr);

			memcpy(msr.pData, &consts, sizeof(consts));
			GetContext(graphics)->Unmap(mpConstantBuffer.Get(), 0u);
		}

	protected:
		ComPtr<ID3D11Buffer> mpConstantBuffer;
		UINT mSlot;
	};

	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::ConstantBuffer;
		using ConstantBuffer<C>::mpConstantBuffer;
		using ConstantBuffer<C>::mSlot;
		using IBindable::GetContext;

	public:
		static shared_ptr<VertexConstantBuffer> Create(Graphics& graphics, const C& consts, UINT slot = 0)
		{
			return ResourceManager::Create<VertexConstantBuffer>(graphics, consts, slot);
		}
		static shared_ptr<VertexConstantBuffer> Create(Graphics& graphics, UINT slot = 0)
		{
			return ResourceManager::Create<VertexConstantBuffer>(graphics, slot);
		}
		static string GenerateUID(const C&, UINT slot)
		{
			return GenerateUID(slot);
		}
		static string GenerateUID(UINT slot = 0)
		{
			return typeid(VertexConstantBuffer).name() + "#"s + to_string(slot);
		}

		virtual void Bind(Graphics& graphics) override
		{
			GetContext(graphics)->VSSetConstantBuffers(0u, 1u, mpConstantBuffer.GetAddressOf());
		}

		string GetUID() const override
		{
			return GenerateUID(mSlot);
		}
	};

	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::ConstantBuffer;
		using ConstantBuffer<C>::mpConstantBuffer;
		using ConstantBuffer<C>::mSlot;
		using IBindable::GetContext;

	public:
		virtual void Bind(Graphics& graphics) override
		{
			GetContext(graphics)->PSSetConstantBuffers(0u, 1u, mpConstantBuffer.GetAddressOf());
		}

		static std::shared_ptr<PixelConstantBuffer> Create(Graphics& graphics, const C& consts, UINT slot = 0)
		{
			return ResourceManager::Create<VertexConstantBuffer>(graphics, consts, slot);
		}
		static std::shared_ptr<PixelConstantBuffer> Create(Graphics& graphics, UINT slot = 0)
		{
			return ResourceManager::Create<VertexConstantBuffer>(graphics, slot);
		}
		static std::string GenerateUID(const C&, UINT slot)
		{
			return GenerateUID(slot);
		}
		static std::string GenerateUID(UINT slot = 0)
		{
			return typeid(PixelConstantBuffer).name() + "#"s + std::to_string(slot);
		}

		string GetUID() const override
		{
			return GenerateUID(slot);
		}
	};

	class TransformConstantBuffer : public IBindable
	{
	public:
		// TransformConstantBuffer(Graphics& graphics, UINT slot = 0u);
		TransformConstantBuffer(Graphics& graphics, const IDrawable& parent)
			: mParent(parent)
		{
			if (!mpVCB)
			{
				mpVCB = make_unique<VertexConstantBuffer<Matrix>>(graphics);
			}
		}
		~TransformConstantBuffer() = default;

		virtual void Bind(Graphics& graphics) override
		{
			Matrix mat = (mParent.GetTransform() * graphics.GetProjection()).Transpose();
			mpVCB->Update(graphics, mat);

			mpVCB->Bind(graphics);
		}
		// virtual void InitReference(const IDrawable& parent) override;

	protected:
		// void UpdateBindImpl(Graphics& graphics, const Transforms& tf);
		// Transforms GetTransforms(Graphics& graphics);

	private:
		static unique_ptr<VertexConstantBuffer<Matrix>> mpVCB;
		const IDrawable& mParent;
	};
}