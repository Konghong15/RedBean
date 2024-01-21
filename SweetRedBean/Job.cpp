#include "pch.h"
#include "Job.h"
#include "Graphics.h"
#include "Step.h"
#include "IDrawable.h"

namespace Rgph
{
	Job::Job(const Step* pStep, const IDrawable* pDrawable)
		: mpDrawable{ pDrawable }
		, mpStep{ pStep }
	{
	}

	void Job::Execute(Graphics& graphics) const
	{
		mpDrawable->Bind(graphics);
		mpStep->Bind(graphics);

		graphics.DrawIndexed(mpDrawable->GetIndexCount());
	}
}