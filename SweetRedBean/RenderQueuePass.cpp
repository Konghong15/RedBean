#include "pch.h"

#include "RenderQueuePass.h"

namespace Rgph
{
	void RenderQueuePass::Accept(Job job)
	{
		mJobs.push_back(job);
	}

	void RenderQueuePass::Execute(Graphics& graphics) const
	{
		BindAll(graphics);

		for (const auto& job : mJobs)
		{
			job.Execute(graphics);
		}
	}

	void RenderQueuePass::Reset()
	{
		mJobs.clear();
	}
}
