#include "pch.h"

#include "IndexBuffer.h"

namespace directXWrapper
{
	bool IndexBuffer::Init(ID3D11Device* device, const vector<unsigned int>& indices)
	{
		mCount = indices.size();

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = static_cast<size_t>(sizeof(unsigned int) * indices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = &indices[0];

		HR(device->CreateBuffer(&desc, &data, mBuffer.GetAddressOf()));

		return true;
	}

	void IndexBuffer::Bind(ID3D11DeviceContext* context) const
	{
		context->IASetIndexBuffer(mBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}