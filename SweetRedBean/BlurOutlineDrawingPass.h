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
		BlurOutlineDrawingPass(Graphics& grapics, std::string name, unsigned int fullWidth, unsigned int fullHeight)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace Bind;
			mRenderTarget = std::make_unique<ShaderInputRenderTarget>(grapics, fullWidth / 2, fullHeight / 2, 0);
			AddBind(VertexShader::Create(grapics, "../SweetRedBean/Solid_VS.hlsl"));
			AddBind(PixelShader::Create(grapics, "../SweetRedBean/Solid_PS.hlsl"));
			AddBind(Stencil::Create(grapics, Stencil::Mode::Mask));
			AddBind(Blender::Create(grapics, false));

			registerSource(DirectBindableSource<Bind::RenderTarget>::Make("scratchOut", mRenderTarget));
		}
		void Execute(Graphics& grapics) const  override
		{
			mRenderTarget->Clear(grapics);
			RenderQueuePass::Execute(grapics);
		}
	};
}