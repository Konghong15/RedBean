#pragma once
#include "Step.h"
#include <vector>

class TechniqueProbe;
class IDrawable;

namespace Rgph
{
	class RenderGraph;
}

class Technique
{
public:
	Technique() = default;
	Technique(std::string name, bool startActive = true);

	void Submit(const IDrawable& drawable) const;
	void Accept(TechniqueProbe& probe);
	void Link(Rgph::RenderGraph& renderGraph);

	void InitReferences(const IDrawable& parent);

	void AddStep(Step step);

	void SetActiveState(bool active_in);

	const std::string& GetName() const;
	bool IsActive() const;

private:
	bool mbIsActive = true;
	std::vector<Step> mSteps;
	std::string mName;
};