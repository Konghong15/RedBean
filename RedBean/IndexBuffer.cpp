#include "pch.h"

#include "IndexBuffer.h"

namespace directXWrapper
{
	bool IndexBuffer::Init(ID3D11Device* device, const vector<unsigned int> indices)
	{
		mStride = sizeof(unsigned int);
		mCount = static_cast<unsigned int>(indices.size());

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (unsigned int)(mStride * mCount);

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = indices.data();

		HR(device->CreateBuffer(&desc, &data, mBuffer.GetAddressOf()));

		return true;
	}

	void IndexBuffer::Bind(ID3D11DeviceContext* context)
	{
		context->IASetIndexBuffer(mBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}