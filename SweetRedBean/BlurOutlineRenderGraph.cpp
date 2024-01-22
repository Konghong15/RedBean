#include "pch.h"

#include "MathUtil.h"
#include "BlurOutlineRenderGraph.h"
#include "BufferClearPass.h"
#include "LambertianPass.h"
#include "OutlineDrawingPass.h"
#include "OutlineMaskGenerationPass.h"
#include "Source.h"
#include "HorizontalBlurPass.h"
#include "VerticalBlurPass.h"
#include "BlurOutlineDrawingPass.h"
#include "RenderTarget.h"
#include "DynamicConstant.h"
#include "Graphics.h"

namespace Rgph
{
	BlurOutlineRenderGraph::BlurOutlineRenderGraph(Graphics& graphics)
		:
		RenderGraph(graphics)
	{
		// 각각 처리할 순서대로 패스를 정의하고,
		{
			auto pass = std::make_unique<BufferClearPass>("clearRT");
			pass->SetSinkLinkage("buffer", "$.backbuffer"); // $ 붙은건 전역 백버퍼를 의미함
			appendPass(std::move(pass));
		}
		{
			auto pass = std::make_unique<BufferClearPass>("clearDS");
			pass->SetSinkLinkage("buffer", "$.masterDepth"); // 전역 깊이 버퍼
			appendPass(std::move(pass));
		}
		{
			auto pass = std::make_unique<LambertianPass>(graphics, "NonPBR");
			pass->SetSinkLinkage("renderTarget", "clearRT.buffer"); // 램버시안 패스를 그리기 위해 clearRT의 소스를 싱크로 연결
			pass->SetSinkLinkage("depthStencil", "clearDS.buffer");
			appendPass(std::move(pass));
		}
		{
			auto pass = std::make_unique<LambertianPass>(graphics, "PBR");
			pass->SetSinkLinkage("renderTarget", "NonPBR.renderTarget"); // 램버시안 패스를 그리기 위해 clearRT의 소스를 싱크로 연결
			pass->SetSinkLinkage("depthStencil", "NonPBR.depthStencil");
			appendPass(std::move(pass));
		}
		{
			auto pass = std::make_unique<OutlineMaskGenerationPass>(graphics, "outlineMask");
			pass->SetSinkLinkage("depthStencil", "PBR.depthStencil");
			appendPass(std::move(pass));
		}

		// setup blur constant buffers
		{
			{
				Dcb::RawLayout l;
				l.Add<Dcb::Integer>("nTaps");
				l.Add<Dcb::Array>("coefficients");
				l["coefficients"].Set<Dcb::Float>(maxRadius * 2 + 1);
				Dcb::Buffer buf{ std::move(l) };
				blurKernel = std::make_shared<Bind::CachingPixelConstantBufferEx>(graphics, buf, 0);
				SetKernelGauss(radius, sigma);
				addGlobalSource(DirectBindableSource<Bind::CachingPixelConstantBufferEx>::Make("blurKernel", blurKernel));
			}
			{
				Dcb::RawLayout l;
				l.Add<Dcb::Bool>("isHorizontal");
				Dcb::Buffer buf{ std::move(l) };
				blurDirection = std::make_shared<Bind::CachingPixelConstantBufferEx>(graphics, buf, 1);
				addGlobalSource(DirectBindableSource<Bind::CachingPixelConstantBufferEx>::Make("blurDirection", blurDirection));
			}
		}

		{
			auto pass = std::make_unique<BlurOutlineDrawingPass>(graphics, "outlineDraw", graphics.GetWidth(), graphics.GetHeight());
			appendPass(std::move(pass));
		}
		{
			auto pass = std::make_unique<HorizontalBlurPass>("horizontal", graphics, graphics.GetWidth(), graphics.GetHeight());
			pass->SetSinkLinkage("scratchIn", "outlineDraw.scratchOut");
			pass->SetSinkLinkage("kernel", "$.blurKernel");
			pass->SetSinkLinkage("direction", "$.blurDirection");
			appendPass(std::move(pass));
		}
		{
			auto pass = std::make_unique<VerticalBlurPass>("vertical", graphics);
			pass->SetSinkLinkage("renderTarget", "PBR.renderTarget");
			pass->SetSinkLinkage("depthStencil", "outlineMask.depthStencil");
			pass->SetSinkLinkage("scratchIn", "horizontal.scratchOut");
			pass->SetSinkLinkage("kernel", "$.blurKernel");
			pass->SetSinkLinkage("direction", "$.blurDirection");
			appendPass(std::move(pass));
		}
		setSinkTarget("backbuffer", "vertical.renderTarget");

		finalize();
	}

	void BlurOutlineRenderGraph::SetKernelGauss(int radius, float sigma)
	{
		assert(radius <= maxRadius);
		auto k = blurKernel->GetBuffer();
		const int nTaps = radius * 2 + 1;
		k["nTaps"] = nTaps;
		float sum = 0.0f;
		for (int i = 0; i < nTaps; i++)
		{
			const auto x = float(i - radius);
			const auto g = gauss(x, sigma);
			sum += g;
			k["coefficients"][i] = g;
		}
		for (int i = 0; i < nTaps; i++)
		{
			k["coefficients"][i] = (float)k["coefficients"][i] / sum;
		}
		blurKernel->SetBuffer(k);
	}
}
