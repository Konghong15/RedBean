#pragma once
#include "RenderQueuePass.h"
#include "Job.h"
#include <vector>
#include "Sink.h"
#include "Source.h"
#include "Stencil.h"

class Graphics;

namespace Rgph
{
	class LambertianPass : public RenderQueuePass
	{
	public:
		LambertianPass(Graphics& gfx, std::string name)
			: RenderQueuePass(std::move(name))
		{
			using namespace Bind;

			// 싱크로 랜더 타겟을 필요로 하고, 소스로 랜더 타겟을 만든다.
			registerSink(DirectBufferSink<RenderTarget>::Make("renderTarget", mRenderTarget));
			registerSink(DirectBufferSink<DepthStencil>::Make("depthStencil", mDepthStencil));
			registerSource(DirectBufferSource<RenderTarget>::Make("renderTarget", mRenderTarget));
			registerSource(DirectBufferSource<DepthStencil>::Make("depthStencil", mDepthStencil));

			AddBind(Stencil::Create(gfx, Stencil::Mode::Off));
		}
	};
}