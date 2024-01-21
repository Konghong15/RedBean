#pragma once

#include "TechniqueProbe.h"
#include "Step.h"

namespace Rgph
{
	class RenderGraph;
}

class StepLinkingProbe : public TechniqueProbe
{
protected:
	void OnSetStep() override { mpStep->Link(mRenderGraph); }

private:
	Rgph::RenderGraph& mRenderGraph;
};