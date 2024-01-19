#pragma once

#include "IBindable.h"
#include "Vertex.h"

namespace bind
{
	class VertexBuffer : public IBindable
	{
	public:
		template <class VertexType>
		VertexBuffer(Graphics& graphics, const vector<VertexType>& vertices)
			: mStride(sizeof(VertexType))
		{
			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0u;
			bd.MiscFlags = 0u;
			bd.ByteWidth = UINT(sizeof(VertexType) * vertices.size());
			bd.StructureByteStride = sizeof(VertexType);

			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = vertices.data();

			GetDevice(graphics)->CreateBuffer(&bd, &sd, &mpVertexBuffer);
		}
		VertexBuffer(Graphics& grapics, const string& tag, dynamic::VertexBuffer& vbuf)
			: mStride((UINT)vbuf.GetLayout().GetSize())
		{
			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0u;
			bd.MiscFlags = 0u;
			bd.ByteWidth = UINT(vbuf.GetSizeBytes());
			bd.StructureByteStride = mStride;
			
			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = vbuf.GetData();

			GetDevice(grapics)->CreateBuffer(&bd, &sd, &mpVertexBuffer);
		}

		//VertexBuffer(Graphics& grapics, const dynamic::VertexBuffer& vertexBuffer);
		~VertexBuffer() = default;

		// static shared_ptr<VertexBuffer> Create(Graphics& graphics, const string tag, const vertexVertexBuffer&vertexBuffer);
		// template <typename... Ignore>
		// static string GenerateUID(const string tag, Ignore&&... ignore);

		virtual void Bind(Graphics& graphics) override
		{
			const UINT offset = 0u;
			GetContext(graphics)->IASetVertexBuffers(0u, 1u, mpVertexBuffer.GetAddressOf(), &mStride, &offset);
		}

		// const vertex::VertexLayout& GetLayout() const;
		// string GetUID() const override;

	private:
		// static std::string generateUID(const std::string& tag);

	private:
		string mTag;
		UINT mStride;
		ComPtr<ID3D11Buffer> mpVertexBuffer;
		// dynamic::VertexLayout mLayout;
	};
}