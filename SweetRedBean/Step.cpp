#include "pch.h"

#include "Step.h"
#include "IDrawable.h"
#include "RenderGraph.h"
#include "TechniqueProbe.h"
#include "RenderQueuePass.h"
#include "Job.h"

Step::Step(std::string targetPassName)
	: mTargetPassName{ std::move(targetPassName) }
{
}

void Step::Submit(const IDrawable& drawable) const
{
	assert(mpTargetPass != nullptr);
	mpTargetPass->Accept(Rgph::Job{ this,&drawable });
}

void Step::InitReferences(const IDrawable& parent)
{
	for (auto& bindable : mBindables)
	{
		bindable->InitReference(parent);
	}
}

Step::Step(const Step& src)
	: mTargetPassName(src.mTargetPassName)
{
	mBindables.reserve(src.mBindables.size());

	for (auto& pb : src.mBindables)
	{
		if (auto* pCloning = dynamic_cast<const ICloningBindable*>(pb.get()))
		{
			mBindables.push_back(pCloning->Clone());
		}
		else
		{
			mBindables.push_back(pb);
		}
	}
}

void Step::AddBindable(std::shared_ptr<IBindable> bind_in)
{
	mBindables.push_back(std::move(bind_in));
}

void Step::Bind(Graphics& graphics) const
{
	for (const auto& bindable : mBindables)
	{
		bindable->Bind(graphics);
	}
}

void Step::Accept(TechniqueProbe& probe)
{
	probe.SetStep(this);

	for (auto& bindable : mBindables)
	{
		bindable->Accept(probe);
	}
}

void Step::Link(Rgph::RenderGraph& renderGraph)
{
	assert(mpTargetPass == nullptr);
	mpTargetPass = &renderGraph.GetRenderQueue(mTargetPassName);
}
