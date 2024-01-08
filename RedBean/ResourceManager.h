#pragma once


class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager() = default;

	void Init(ID3D11Device* device) { mDevice = device; }

	ComPtr<ID3D11ShaderResourceView> CreateTexture(const std::wstring& filename);

public:
	ComPtr<ID3D11Device> mDevice;
	std::map<std::wstring, ComPtr<ID3D11ShaderResourceView>> mTextureSRVs;
};

