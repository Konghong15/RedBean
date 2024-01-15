#pragma once

namespace directXWrapper
{
	class IndexBuffer
	{
	public:
		IndexBuffer() = default;
		~IndexBuffer() = default;

		bool Init(ID3D11Device* device, const vector<unsigned int>& indices);
		void Bind(ID3D11DeviceContext* context) const;

		ComPtr<ID3D11Buffer> GetComPtr() const { return mBuffer; }
		UINT GetCount() const { return mCount; }

	private:
		ComPtr<ID3D11Buffer> mBuffer;

		UINT mCount = 0;
	};
}