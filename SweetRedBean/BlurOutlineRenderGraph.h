#pragma once

#include "RenderGraph.h"
#include "ConstantBufferEx.h"

class Graphics;
class IBindable;

namespace Bind
{
	class RenderTarget;
}

namespace Rgph
{
	class BlurOutlineRenderGraph : public RenderGraph
	{
	public:
		BlurOutlineRenderGraph(Graphics& grapics);
	private:
		// private functions
		void SetKernelGauss(int radius, float sigma);
		// private data
		static constexpr int maxRadius = 7;
		static constexpr int radius = 4;
		static constexpr float sigma = 2.0f;
		std::shared_ptr<Bind::CachingPixelConstantBufferEx> blurKernel;
		std::shared_ptr<Bind::CachingPixelConstantBufferEx> blurDirection;
	};
}
