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
	class VerticalBlurPass : public FullscreenPass
	{
	public:
		VerticalBlurPass( std::string name,Graphics& gfx );
		void Execute( Graphics& gfx ) const  override;

	private:
		std::shared_ptr<Bind::CachingPixelConstantBufferEx> direction;
	};
}