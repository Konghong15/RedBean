#pragma once

namespace directXWrapper
{
	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		~VertexBuffer() = default;

		template <typename T>
		bool Init(ID3D11Device* device, const vector<T>& vertices, unsigned int slot = 0, bool cpuWrite = false, bool gpuWrite = false);
		inline void Bind(ID3D11DeviceContext* context);

		ComPtr<ID3D11Buffer> GetComPtr() { return mBuffer; }
		UINT GetStride() { return mStride; }
		UINT GetOffset() { return mOffset; }
		UINT GetCount() { return mCount; }
		UINT GetSlot() { return mSlot; }

	private:
		ComPtr<ID3D11Buffer> mBuffer;

		UINT mStride = 0;
		UINT mOffset = 0;
		UINT mCount = 0;
		UINT mSlot = 0;
	};

	template <typename T>
	bool VertexBuffer::Init(ID3D11Device* device, const vector<T>& vertices, unsigned int slot, bool cpuWrite, bool gpuWrite)
	{
		mStride = sizeof(T);
		mCount = static_cast<unsigned int>(vertices.size());

		mSlot = slot;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (unsigned int)(mStride * mCount);

		if (cpuWrite == false && gpuWrite == false)
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE; // CPU Read, GPU Read
		}
		else if (cpuWrite == true && gpuWrite == false)
		{
			desc.Usage = D3D11_USAGE_DYNAMIC; // CPU Write, GPU Read
			desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		}
		else if (cpuWrite == false && gpuWrite == true) // CPU Read, GPU Write
		{
			desc.Usage = D3D11_USAGE_DEFAULT;
		}
		else
		{
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		}

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data();

		HR(device->CreateBuffer(&desc, &data, mBuffer.GetAddressOf()));

		return true;
	}

	void VertexBuffer::Bind(ID3D11DeviceContext* context)
	{
		context->IASetVertexBuffers(mSlot, 1, mBuffer.GetAddressOf(), &mStride, &mOffset);
	}
}

