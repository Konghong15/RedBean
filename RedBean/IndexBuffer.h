#pragma once

namespace directXWrapper
{
	class IndexBuffer
	{
	public:
		IndexBuffer() = default;
		~IndexBuffer() = default;
		
		bool Init(ID3D11Device* device, const vector<unsigned int> indices);
		void Bind(ID3D11DeviceContext* context);

		ComPtr<ID3D11Buffer> GetComPtr() { return mBuffer; }
		UINT GetStride() { return mStride; }
		UINT GetOffset() { return mOffset; }
		UINT GetCount() { return mCount; }

	private:
		ComPtr<ID3D11Buffer> mBuffer;

		UINT mStride = 0;
		UINT mOffset = 0;
		UINT mCount = 0;
	};
}