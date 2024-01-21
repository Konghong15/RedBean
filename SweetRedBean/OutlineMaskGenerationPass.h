#pragma once

#include "RenderQueuePass.h"
#include "Job.h"
#include "NullPixelShader.h"
#include "VertexShader.h"
#include "Stencil.h"
#include "Rasterizer.h"

class Graphics;

namespace Rgph
{
	class OutlineMaskGenerationPass : public RenderQueuePass
	{
	public:
		OutlineMaskGenerationPass(Graphics& gfx, std::string name)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace Bind;
			registerSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", mDepthStencil));
			registerSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", mDepthStencil));
			AddBind(VertexShader::Create(gfx, "Solid_VS.cso"));
			AddBind(NullPixelShader::Create(gfx));
			AddBind(Stencil::Create(gfx, Stencil::Mode::Write));
			AddBind(Rasterizer::Create(gfx, false));
		}
	};
}