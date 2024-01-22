#include "pch.h"

#include "VerticalBlurPass.h"
#include "Sink.h"
#include "Source.h"
#include "PixelShader.h"
#include "Blender.h"
#include "Stencil.h"
#include "Sampler.h"

using namespace Bind;

namespace Rgph
{
	VerticalBlurPass::VerticalBlurPass(std::string name, Graphics& grapics)
		:
		FullscreenPass(std::move(name), grapics)
	{
		AddBind(PixelShader::Create(grapics, "../SweetRedBean/BlurOutline_PS.hlsl"));
		AddBind(Blender::Create(grapics, true));
		AddBind(Stencil::Create(grapics, Stencil::Mode::Mask));
		AddBind(Sampler::Create(grapics, Sampler::Type::Bilinear, true));

		addBindSink<RenderTarget>("scratchIn");
		addBindSink<CachingPixelConstantBufferEx>("kernel");
		registerSink(DirectBindableSink<CachingPixelConstantBufferEx>::Make("direction", direction));
		registerSink(DirectBufferSink<RenderTarget>::Make("renderTarget", mRenderTarget));
		registerSink(DirectBufferSink<DepthStencil>::Make("depthStencil", mDepthStencil));

		registerSource(DirectBufferSource<RenderTarget>::Make("renderTarget", mRenderTarget));
		registerSource(DirectBufferSource<DepthStencil>::Make("depthStencil", mDepthStencil));
	}

	void VerticalBlurPass::Execute(Graphics& grapics) const
	{
		auto buf = direction->GetBuffer();
		buf["isHorizontal"] = false;
		direction->SetBuffer(buf);

		direction->Bind(grapics);
		FullscreenPass::Execute(grapics);
	}
}
