#include "pch.h"

#include "ScaleOutlineRenderGraph.h"
#include "BufferClearPass.h"
#include "LambertianPass.h"
#include "OutlineDrawingPass.h"
#include "OutlineMaskGenerationPass.h"

namespace Rgph
{
	ScaleOutlineRenderGraph::ScaleOutlineRenderGraph(Graphics& grapics)
		: RenderGraph(grapics)
	{
		{
			auto pass = std::make_unique<BufferClearPass>("clearRT");
			pass->SetSinkLinkage("buffer", "$.backbuffer");
			appendPass(std::move(pass));
		}
		{
			auto pass = std::make_unique<BufferClearPass>("clearDS");
			pass->SetSinkLinkage("buffer", "$.masterDepth");
			appendPass(std::move(pass));
		}
		{
			auto pass = std::make_unique<LambertianPass>(grapics, "lambertian");
			pass->SetSinkLinkage("renderTarget", "clearRT.buffer");
			pass->SetSinkLinkage("depthStencil", "clearDS.buffer");
			appendPass(std::move(pass));
		}
		{
			auto pass = std::make_unique<OutlineMaskGenerationPass>(grapics, "outlineMask");
			pass->SetSinkLinkage("depthStencil", "lambertian.depthStencil");
			appendPass(std::move(pass));
		}
		{
			auto pass = std::make_unique<OutlineDrawingPass>(grapics, "outlineDraw");
			pass->SetSinkLinkage("renderTarget", "lambertian.renderTarget");
			pass->SetSinkLinkage("depthStencil", "outlineMask.depthStencil");
			appendPass(std::move(pass));
		}

		setSinkTarget("backbuffer", "outlineDraw.renderTarget");
		finalize();
	}
}
