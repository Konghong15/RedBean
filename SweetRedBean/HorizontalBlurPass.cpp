#include "pch.h"

#include "HorizontalBlurPass.h"
#include "PixelShader.h"
#include "RenderTarget.h"
#include "Sink.h"
#include "Source.h"
#include "Blender.h"
#include "Sampler.h"
#include "ConstantBufferEx.h"

using namespace Bind;

namespace Rgph
{
	HorizontalBlurPass::HorizontalBlurPass(std::string name, Graphics& grapics, unsigned int fullWidth, unsigned int fullHeight)
		:
		FullscreenPass(std::move(name), grapics)
	{
		AddBind(PixelShader::Create(grapics, "../SweetRedBean/BlurOutline_PS.hlsl"));
		AddBind(Blender::Create(grapics, false));
		AddBind(Sampler::Create(grapics, Sampler::Type::Point, true));

		addBindSink<Bind::RenderTarget>("scratchIn");
		addBindSink<Bind::CachingPixelConstantBufferEx>("kernel");
		registerSink(DirectBindableSink<CachingPixelConstantBufferEx>::Make("direction", direction));

		// the renderTarget is internally sourced and then exported as a IBindable
		mRenderTarget = std::make_shared<Bind::ShaderInputRenderTarget>(grapics, fullWidth / 2, fullHeight / 2, 0u);
		registerSource(DirectBindableSource<RenderTarget>::Make("scratchOut", mRenderTarget));
	}

	// this override is necessary because we cannot (yet) link input bindables directly into
	// the container of bindables (mainly because vector growth buggers references)
	void HorizontalBlurPass::Execute(Graphics& grapics) const
	{
		auto buf = direction->GetBuffer();
		buf["isHorizontal"] = true;
		direction->SetBuffer(buf);

		direction->Bind(grapics);
		FullscreenPass::Execute(grapics);
	}
}
