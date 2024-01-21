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
		HorizontalBlurPass(std::string name, Graphics& gfx, unsigned int fullWidth, unsigned int fullHeight);
		void Execute(Graphics& gfx) const  override;
	private:
		std::shared_ptr<Bind::CachingPixelConstantBufferEx> direction;
	};
}