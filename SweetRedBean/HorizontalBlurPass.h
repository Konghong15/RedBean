#pragma once

#include "FullscreenPass.h"
#include "ConstantBufferEx.h"

class Graphics;

namespace Bind
{
	class PixelShader;
	class RenderTarget;
}

namespace Rgph
{
	class HorizontalBlurPass : public FullscreenPass
	{
	public:
		HorizontalBlurPass(std::string name, Graphics& grapics, unsigned int fullWidth, unsigned int fullHeight);
		void Execute(Graphics& grapics) const  override;
	private:
		std::shared_ptr<Bind::CachingPixelConstantBufferEx> direction;
	};
}