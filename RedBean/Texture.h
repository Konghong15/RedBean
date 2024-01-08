#pragma once

namespace directXWrapper
{
	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		bool Init(ID3D11Device* device, const wstring& path);
		bool Init(ID3D11Device* device, const std::vector<wstring>& paths);

		virtual void Bind(ID3D11DeviceContext* context);

		ComPtr<ID3D11ShaderResourceView> GetComPtr() { return mSRV; }
		const Vector2& GetSize() const { return mSize; }
		const ScratchImage& GetScratchImage() const { return mScratchImage; }

	private:
		wstring mPath;
		ComPtr<ID3D11ShaderResourceView> mSRV;
		Vector2 mSize;
		ScratchImage mScratchImage;

		// 바인딩 정보
	};
}