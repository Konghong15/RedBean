#pragma once

#include "IBindable.h"
#include "BindableResourceManager.h"

namespace bind
{
	class IndexBuffer : public IBindable
	{
	public:
		IndexBuffer(Graphics& graphics, const std::vector<unsigned short>& indices)
			: IndexBuffer(graphics, "?", indices)
		{
		}
		IndexBuffer(Graphics& graphics, std::string tag, const std::vector<unsigned short>& indices)
			: mTag(tag)
			, mCount((UINT)indices.size())
		{
			D3D11_BUFFER_DESC ibd = {};
			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			ibd.Usage = D3D11_USAGE_DEFAULT;
			ibd.CPUAccessFlags = 0u;
			ibd.MiscFlags = 0u;
			ibd.ByteWidth = UINT(mCount * sizeof(unsigned short));
			ibd.StructureByteStride = sizeof(unsigned short);

			D3D11_SUBRESOURCE_DATA isd = {};
			isd.pSysMem = indices.data();

			(GetDevice(graphics)->CreateBuffer(&ibd, &isd, &mpIndexBuffer));
		}
		~IndexBuffer() = default;

		static shared_ptr<IndexBuffer> Resolve(Graphics& graphics, const std::string& tag, const vector<unsigned short>& indices)
		{
			return ResourceManager::Create<IndexBuffer>(graphics, tag, indices);
		}
		template<typename...Ignore>
		static string GenerateUID(const std::string& tag, Ignore&&...ignore)
		{
			return generateUID(tag);
		}

		virtual void Bind(Graphics& graphics) override
		{
			GetContext(graphics)->IASetIndexBuffer(mpIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
		}

		UINT GetCount() const
		{
			return mCount;
		}
		string GetUID() const  override
		{
			return generateUID(mTag);
		}

	private:
		static std::string generateUID(const std::string& tag)
		{
			return typeid(IndexBuffer).name() + "#"s + tag;
		}

	protected:
		std::string mTag;
		UINT mCount;
		ComPtr<ID3D11Buffer> mpIndexBuffer;
	};
}