#pragma once

namespace directXWrapper
{
	template <typename T>
	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;
		~ConstantBuffer() = default;

		bool Init(ID3D11Device* device);

		ComPtr<ID3D11Buffer> GetComPtr() { return mBuffer; }

	private:
		ComPtr<ID3D11Buffer> mBuffer;
	};

	template <typename T>
	bool ConstantBuffer<T>::Init(ID3D11Device* device)
	{
		static_assert(sizeof(T) % 16 == 0, "constant buffer must be align 16 byte");

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC; // CPU_Write + GPU_Read
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HR(device->CreateBuffer(&desc, nullptr, mBuffer.GetAddressOf()));

		return true;
	}
}