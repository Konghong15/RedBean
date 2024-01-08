#include "pch.h"

#include "ResourceManager.h"

ComPtr<ID3D11ShaderResourceView> ResourceManager::CreateTexture(const std::wstring& filename)
{
	auto find = mTextureSRVs.find(filename);

	if (find != mTextureSRVs.end())
	{
		return find->second;
	}

	ComPtr<ID3D11ShaderResourceView> srv;
	HR(common::D3DHelper::CreateTextureFromFile(mDevice.Get(), filename.c_str(), srv.GetAddressOf()));

	mTextureSRVs.insert({ filename, srv });

	return srv;
}