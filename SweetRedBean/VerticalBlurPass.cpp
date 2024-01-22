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
	VerticalBlurPass::VerticalBlurPass(std::string name, Graphics& gfx)
		:
		FullscreenPass(std::move(name), gfx)
	{
		AddBind(PixelShader::Create(gfx, "../SweetRedBean/BlurOutline_PS.hlsl"));
		AddBind(Blender::Create(gfx, true));
		AddBind(Stencil::Create(gfx, Stencil::Mode::Mask));
		AddBind(Sampler::Create(gfx, Sampler::Type::Bilinear, true));

		addBindSink<RenderTarget>("scratchIn");
		addBindSink<CachingPixelConstantBufferEx>("kernel");
		registerSink(DirectBindableSink<CachingPixelConstantBufferEx>::Make("direction", direction));
		registerSink(DirectBufferSink<RenderTarget>::Make("renderTarget", mRenderTarget));
		registerSink(DirectBufferSink<DepthStencil>::Make("depthStencil", mDepthStencil));

		registerSource(DirectBufferSource<RenderTarget>::Make("renderTarget", mRenderTarget));
		registerSource(DirectBufferSource<DepthStencil>::Make("depthStencil", mDepthStencil));
	}

	void VerticalBlurPass::Execute(Graphics& gfx) const
	{
		auto buf = direction->GetBuffer();
		buf["isHorizontal"] = false;
		direction->SetBuffer(buf);

		direction->Bind(gfx);
		FullscreenPass::Execute(gfx);
	}
}
