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
		// �������� ����� �ҽ��� ��ũ
		// ����ۿ� ���� ����Ÿ�ٰ� ���̹��۴� �ݵ�� �����
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
		// todo : ����ó�� �������� ����
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

		// �����׷����� ����� ��� �н��� ���� ������ Ȯ����
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

		// ���� �׷������� ������ �н��� ������ �� ����.
		for (const auto& p : mPasses)
		{
			if (pass->GetName() == p->GetName())
			{
				assert(false);
			}
		}

		// �н��� �߰��� ������ �ҽ�/��ũ ������ ������
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

		// ��ũ�� ��ȸ�ϸ� �ҽ��� �������ش�.
		for (auto& sink : pass.GetSinks())
		{
			const auto& inputSourcePassName = sink->GetPassName();
			
			// ���� �ҽ��� ���
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
			else // �н��� �ҽ��� ���
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

		// ���ῡ �����ϸ� �ȵ�
		
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