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
		OutlineDrawingPass(Graphics& grapics, std::string name)
			:
			RenderQueuePass(std::move(name))
		{
			using namespace Bind;
			registerSink(DirectBufferSink<Bind::RenderTarget>::Make("renderTarget", mRenderTarget));
			registerSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", mDepthStencil));
			registerSource(DirectBufferSource<Bind::RenderTarget>::Make("renderTarget", mRenderTarget));
			registerSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", mDepthStencil));
			AddBind(VertexShader::Create(grapics, "../SweetRedBean/Solid_VS.hlsl"));
			AddBind(PixelShader::Create(grapics, "../SweetRedBean/Solid_PS.hlsl"));
			AddBind(Stencil::Create(grapics, Stencil::Mode::Mask));
			AddBind(Rasterizer::Create(grapics, false));
		}
	};
}