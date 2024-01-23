#pragma once
#include "IBindable.h"

namespace Bind
{
	class Sampler : public IBindable
	{
	public:
		enum class Type
		{
			Anisotropic,
			Bilinear,
			Point,
		};

	public:
		Sampler(Graphics& grapics, Type type, bool reflect, UINT slot = 0u);
		//Sampler(Graphics& graphics, Type type, D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP);
		virtual ~Sampler() = default;

		static std::shared_ptr<Sampler> Create(Graphics& grapics, Type type = Type::Anisotropic, bool reflect = false, UINT slot = 0u);
		// static std::shared_ptr<Sampler> Create(Graphics& graphics, Type type = Type::Anisotropic, D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP);
		static std::string GenerateUID(Type type, bool reflect, UINT slot = 0u);

		void Bind(Graphics& graphics) override;

		std::string GetUID() const override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mpSampler;
		Type mType;
		UINT mSlot;
		bool mbIsReflect;
	};
}
