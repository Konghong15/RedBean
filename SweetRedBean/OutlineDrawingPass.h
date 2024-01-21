#pragma once
#include "RenderQueuePass.h"
#include "Job.h"
#include <vector>
#include "PixelShader.h"
#include "VertexShader.h"
#include "Stencil.h"
#include "Rasterizer.h"

class Graphics;

namespace Rgph
{
	class OutlineDrawingPass : public RenderQueuePass
	{
	public:
		OutlineDrawingPass(Graphics& gfx, std::string name)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace Bind;
			registerSink(DirectBufferSink<Bind::RenderTarget>::Make("renderTarget", mRenderTarget));
			registerSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", mDepthStencil));
			registerSource(DirectBufferSource<Bind::RenderTarget>::Make("renderTarget", mRenderTarget));
			registerSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", mDepthStencil));
			AddBind(VertexShader::Create(gfx, "Solid_VS.cso"));
			AddBind(PixelShader::Create(gfx, "Solid_PS.cso"));
			AddBind(Stencil::Create(gfx, Stencil::Mode::Mask));
			AddBind(Rasterizer::Create(gfx, false));
		}
	};
}