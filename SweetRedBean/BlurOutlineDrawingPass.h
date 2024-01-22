#pragma once
#include "RenderQueuePass.h"
#include "Job.h"
#include <vector>
#include "PixelShader.h"
#include "VertexShader.h"
#include "Stencil.h"
#include "Rasterizer.h"
#include "Source.h"
#include "RenderTarget.h"
#include "Blender.h"

class Graphics;

namespace Rgph
{
	class BlurOutlineDrawingPass : public RenderQueuePass
	{
	public:
		BlurOutlineDrawingPass(Graphics& gfx, std::string name, unsigned int fullWidth, unsigned int fullHeight)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace Bind;
			mRenderTarget = std::make_unique<ShaderInputRenderTarget>(gfx, fullWidth / 2, fullHeight / 2, 0);
			AddBind(VertexShader::Create(gfx, "../SweetRedBean/Solid_VS.hlsl"));
			AddBind(PixelShader::Create(gfx, "../SweetRedBean/Solid_PS.hlsl"));
			AddBind(Stencil::Create(gfx, Stencil::Mode::Mask));
			AddBind(Blender::Create(gfx, false));
			registerSource(DirectBindableSource<Bind::RenderTarget>::Make("scratchOut", mRenderTarget));
		}
		void Execute(Graphics& gfx) const  override
		{
			mRenderTarget->Clear(gfx);
			RenderQueuePass::Execute(gfx);
		}
	};
}