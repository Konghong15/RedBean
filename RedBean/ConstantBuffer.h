#pragma once

namespace directXWrapper
{
	template <typename T>
	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;
		~ConstantBuffer() = default;

		bool Init(ID3D11Device* device, bool cpuWrite = false, bool gpuWrite = true);
		void UpdateSubResource(ID3D11DeviceContext* context, void* data);
		void Destroy();

		ComPtr<ID3D11Buffer> GetComPtr() { return mBuffer; }

	private:
		ComPtr<ID3D11Buffer> mBuffer;
	};

	template <typename T>
	bool ConstantBuffer<T>::Init(ID3D11Device* device, bool cpuWrite, bool gpuWrite)
	{
		static_assert(sizeof(T) % 16 == 0, "constant buffer must be aligned by 16 bytes");

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);

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

		HR(device->CreateBuffer(&desc, nullptr, mBuffer.GetAddressOf()));

		return true;
	}

	template <typename T>
	void ConstantBuffer<T>::UpdateSubResource(ID3D11DeviceContext* context, void* data)
	{
		context->UpdateSubresource(mBuffer.Get(), 0, 0, data, 0, 0);
	}

	template <typename T>
	void ConstantBuffer<T>::Destroy()
	{
		mBuffer = nullptr;
	}
}