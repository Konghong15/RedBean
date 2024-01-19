#pragma once

namespace resource
{
	class Texture final
	{
	public:
		Texture() = default;
		~Texture() = default;
		Texture(const Texture& other) = default;
		Texture& operator=(const Texture& other) = default;

		bool Init(ID3D11Device* device, const wstring& path);
		bool Init(ID3D11Device* device, const std::vector<wstring>& paths);

		const wstring& GetPath() const { return mPath; };
		ComPtr<ID3D11ShaderResourceView> GetComPtr() const { return mSRV; }
		const Vector2& GetSize() const { return mSize; }
		const ScratchImage& GetScratchImage() const { return mScratchImage; }

	private:
		wstring mPath;
		ComPtr<ID3D11ShaderResourceView> mSRV;
		Vector2 mSize;
		ScratchImage mScratchImage;
	};
}