#include "pch.h"

#include "Texture.h"

namespace resource
{
	bool Texture::Init(ID3D11Device* device, const wstring& path)
	{
		mPath = path;

		DirectX::TexMetadata md;
		HRESULT hr = LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, mScratchImage);

		if (FAILED(hr))
		{
			hr = LoadFromTGAFile(path.c_str(), &md, mScratchImage);
		}
		if (FAILED(hr))
		{
			hr = LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, &md, mScratchImage);
		}
		if (FAILED(hr))
		{
			hr = LoadFromHDRFile(path.c_str(), &md, mScratchImage);
		}

		HR(CreateShaderResourceView(device, mScratchImage.GetImages(), mScratchImage.GetImageCount(), md, mSRV.GetAddressOf()));
		mSize = { (float)md.width, (float)md.height };

		return true;
	}

	bool Texture::Init(ID3D11Device* device, const std::vector<wstring>& paths)
	{

		return true;
	}
}