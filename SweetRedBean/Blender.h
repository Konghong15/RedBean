#pragma once

#include "IBindable.h"
#include "BindableResourceManager.h"

namespace bind
{
	class Blender : public IBindable
	{
	public:
		Blender(Graphics& graphics, bool blending, std::optional<float> factor = {})
			: mBlending(blending)
		{
			if (factor.has_value())
			{
				mFactors.emplace();
				mFactors->fill(factor.value());
			}

			D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
			auto& brt = blendDesc.RenderTarget[0];

			if (blending)
			{
				brt.BlendEnable = true;

				if (factor)
				{
					brt.SrcBlend = D3D11_BLEND_BLEND_FACTOR;
					brt.DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
				}
				else
				{
					brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
					brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				}
			}

			GetDevice(graphics)->CreateBlendState(&blendDesc, &mpBlender);
		}

		static shared_ptr<Blender> Create(Graphics& graphics, bool blending, optional<float> factor = {})
		{
			return ResourceManager::Create<Blender>(graphics, blending, factor);
		}

		static string GenerateUID(bool blending, optional<float> factor)
		{
			return typeid(Blender).name()
				+ "#"s + (blending ? "b"s : "n"s)
				+ (factor ? "#f"s + std::to_string(*factor) : "");
		}

		void Bind(Graphics& graphics) override
		{
			const float* data = mFactors ? mFactors->data() : nullptr;
			GetContext(graphics)->OMSetBlendState(mpBlender.Get(), data, 0xFFFFFFFFu);
		}

		inline void SetFactor(float factor)
		{
			assert(mFactors);
			mFactors->fill(factor);
		}

		inline float GetFactor() const
		{
			assert(mFactors);
			return mFactors->front();
		}
		inline string GetUID() const override
		{
			return GenerateUID(mBlending, mFactors ? mFactors->front() : optional<float>{});
		}

	protected:
		ComPtr<ID3D11BlendState> mpBlender;
		bool mBlending;
		optional<array<float, 4>> mFactors;
	};
}
