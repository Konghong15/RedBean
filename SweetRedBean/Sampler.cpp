#include "pch.h"

#include "Sampler.h"
#include "BindableResourceManager.h"

namespace Bind
{
	Sampler::Sampler(Graphics& gfx, Type type, bool reflect)
		: mType(type)
		, mbIsReflect(reflect)
	{
		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };

		samplerDesc.Filter = [type]() {
			switch (type)
			{
			case Type::Anisotropic:
				return D3D11_FILTER_ANISOTROPIC;
			case Type::Point:
				return D3D11_FILTER_MIN_MAG_MIP_POINT;
			case Type::Bilinear:
				return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			default:
				assert(false);
			}
		}();

		samplerDesc.AddressU = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

		GetDevice(gfx)->CreateSamplerState(&samplerDesc, &mpSampler);
	}

	//Sampler::Sampler(Graphics& graphics, Type type, D3D11_TEXTURE_ADDRESS_MODE addressMode)
	//	: mType(type)
	//	, mAddressMode(addressMode)
	//{
	//	D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };

	//	samplerDesc.Filter = [type]() {
	//		switch (type)
	//		{
	//		case Type::Anisotropic:
	//			return D3D11_FILTER_ANISOTROPIC;
	//		case Type::Point:
	//			return D3D11_FILTER_MIN_MAG_MIP_POINT;
	//		case Type::Bilinear:
	//			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//		default:
	//			assert(false);
	//		}
	//	}();

	//	samplerDesc.AddressU = addressMode;
	//	samplerDesc.AddressV = addressMode;
	//	samplerDesc.AddressW = addressMode;
	//	samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

	//	GetDevice(graphics)->CreateSamplerState(&samplerDesc, &mpSampler);
	//}

	void Sampler::Bind(Graphics& graphics)
	{
		GetContext(graphics)->PSSetSamplers(0, 1, mpSampler.GetAddressOf());
	}

	std::shared_ptr<Sampler> Sampler::Create(Graphics& graphics, Type type, bool reflect)
	{
		return ResourceManager::Create<Sampler>(graphics, type, reflect);
	}
	// std::shared_ptr<Sampler> Sampler::Create(Graphics& graphics, Type type, D3D11_TEXTURE_ADDRESS_MODE addressMode)
	// {
	// 	return ResourceManager::Create<Sampler>(graphics, type, addressMode);
	// }
	std::string Sampler::GenerateUID(Type type, bool reflect)
	{
		using namespace std::string_literals;
		return typeid(Sampler).name() + "#"s + std::to_string((int)type) + (reflect ? "R"s : "W"s);
	}
	std::string Sampler::GetUID() const
	{
		return GenerateUID(mType, mbIsReflect);
	}
}