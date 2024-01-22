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
		OutlineMaskGenerationPass(Graphics& grapics, std::string name)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace Bind;
			registerSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", mDepthStencil));
			registerSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", mDepthStencil));
			AddBind(VertexShader::Create(grapics, "../SweetRedBean/Solid_VS.hlsl"));
			AddBind(NullPixelShader::Create(grapics));
			AddBind(Stencil::Create(grapics, Stencil::Mode::Write));
			AddBind(Rasterizer::Create(grapics, false));
		}
	};
}