#include "pch.h"

#include "RenderGraph.h"
#include "Pass.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "BindableCommon.h"
#include "RenderQueuePass.h"
#include "Sink.h"
#include "Source.h"
#include "Util.h"

namespace Rgph
{
	RenderGraph::RenderGraph(Graphics& graphics)
		: mBackBufferTarget(graphics.GetRenderTarget())
		, mMasterDepth(std::make_shared<Bind::OutputOnlyDepthStencil>(graphics))
	{
		// 전역으로 사용할 소스와 싱크
		// 백버퍼에 대한 랜더타겟과 깊이버퍼는 반드시 연결됨
		addGlobalSource(DirectBufferSource<Bind::RenderTarget>::Make("backbuffer", mBackBufferTarget));
		addGlobalSource(DirectBufferSource<Bind::DepthStencil>::Make("masterDepth", mMasterDepth));
		addGlobalSink(DirectBufferSink<Bind::RenderTarget>::Make("backbuffer", mBackBufferTarget));
	}

	void RenderGraph::Execute(Graphics& graphics)
	{
		assert(mbIsFinalized);

		for (auto& p : mPasses)
		{
			p->Execute(graphics);
		}
	}

	void RenderGraph::Reset()
	{
		assert(mbIsFinalized);

		for (auto& p : mPasses)
		{
			p->Reset();
		}
	}

	RenderQueuePass& RenderGraph::GetRenderQueue(const std::string& passName)
	{
		// todo : 예외처리 로직으로 수정
		try
		{
			for (const auto& p : mPasses)
			{
				if (p->GetName() == passName)
				{
					return dynamic_cast<RenderQueuePass&>(*p);
				}
			}
		}
		catch (std::bad_cast&)
		{
			assert(false);
		}
		assert(false);
	}

	void RenderGraph::finalize()
	{
		assert(!mbIsFinalized);

		// 랜더그래프에 연결된 모든 패스에 대해 연결을 확인함
		for (const auto& p : mPasses)
		{
			p->Finalize();
		}

		linkGlobalSinks();
		mbIsFinalized = true;
	}


	void RenderGraph::addGlobalSource(std::unique_ptr<Source> out)
	{
		mGlobalSources.push_back(std::move(out));
	}

	void RenderGraph::addGlobalSink(std::unique_ptr<Sink> in)
	{
		mGlobalSinks.push_back(std::move(in));
	}

	void RenderGraph::appendPass(std::unique_ptr<Pass> pass)
	{
		assert(!mbIsFinalized);

		// 랜더 그래프에는 동일한 패스가 존재할 수 없다.
		for (const auto& p : mPasses)
		{
			if (pass->GetName() == p->GetName())
			{
				assert(false);
			}
		}

		// 패스가 추가될 때마다 소스/싱크 매핑을 시행함
 		linkSinks(*pass);

		mPasses.push_back(std::move(pass));
	}

	void RenderGraph::setSinkTarget(const std::string& sinkName, const std::string& target)
	{
		const auto finder = [&sinkName](const std::unique_ptr<Sink>& p) -> bool
		{
			return p->GetRegisteredName() == sinkName;
		};

		const auto finded = std::find_if(mGlobalSinks.begin(), mGlobalSinks.end(), finder);

		if (finded == mGlobalSinks.end())
		{
			assert(false);
		}

		auto targetSplit = Util::SplitString(target, ".");

		if (targetSplit.size() != 2u)
		{
			assert(false);
		}

		(*finded)->SetTarget(targetSplit[0], targetSplit[1]);
	}

	void RenderGraph::linkSinks(Pass& pass)
	{

		// 싱크를 순회하며 소스를 매핑해준다.
		for (auto& sink : pass.GetSinks())
		{
			const auto& inputSourcePassName = sink->GetPassName();
			
			// 전역 소스일 경우
			if (inputSourcePassName == "$")
			{
				bool bound = false;

				for (auto& source : mGlobalSources)
				{
					if (source->GetName() == sink->GetOutputName())
					{
						sink->Bind(*source);
						bound = true;

						break;
					}
				}

				if (!bound)
				{
					assert(false);
				}
			}
			else // 패스의 소스일 경우
			{
				bool bound = false;

				for (auto& pass : mPasses)
				{
					if (pass->GetName() == inputSourcePassName)
					{
						auto& source = pass->GetSource(sink->GetOutputName());
						sink->Bind(source);
						bound = true;

						break;
					}
				}

				if (!bound)
				{
					assert(false);
				}
			}
		}

		// 연결에 실패하면 안됨
		
	}

	void RenderGraph::linkGlobalSinks()
	{
		for (auto& sink : mGlobalSinks)
		{
			const auto& inputSourcePassName = sink->GetPassName();

			for (auto& existingPass : mPasses)
			{
				if (existingPass->GetName() == inputSourcePassName)
				{
					auto& source = existingPass->GetSource(sink->GetOutputName());
					sink->Bind(source);
					break;
				}
			}
		}
	}


}