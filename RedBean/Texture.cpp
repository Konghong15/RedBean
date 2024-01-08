#include "pch.h"

#include "Texture.h"

namespace directXWrapper
{
	bool Texture::Init(ID3D11Device* device, const wstring& path)
	{
		mPath = path;

		DirectX::TexMetadata md;
		HR(LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, mScratchImage));
		HR(CreateShaderResourceView(device, mScratchImage.GetImages(), mScratchImage.GetImageCount(), md, mSRV.GetAddressOf()));
		mSize = { (float)md.width, (float)md.height };

		return true;
	}

	bool Texture::Init(ID3D11Device* device, const std::vector<wstring>& paths)
	{

		return true;
	}

	void Texture::Bind(ID3D11DeviceContext* context)
	{
		context->VSSetShaderResources(0, 1, mSRV.GetAddressOf());
		context->PSSetShaderResources(0, 1, mSRV.GetAddressOf());
	}
}