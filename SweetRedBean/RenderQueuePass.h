#pragma once
#include "BindingPass.h"
#include "Job.h"
#include <vector>

/*
Job을 등록할 수 있는 Pass
*/

namespace Rgph
{
	class RenderQueuePass : public BindingPass
	{
	public:
		using BindingPass::BindingPass;

		void Accept( Job job );
		void Execute( Graphics& graphics ) const  override;
		void Reset()  override;

	private:
		std::vector<Job> mJobs;
	};
}