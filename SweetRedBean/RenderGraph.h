#pragma once

class Graphics;

namespace Bind
{
	class RenderTarget;
	class DepthStencil;
}

/*
랜더 그래프는 프레임이 어떻게 그려질지에 대한 정보를 관리한다.
*/

namespace Rgph
{
	class Pass;
	class RenderQueuePass;
	class Source;
	class Sink;

	class RenderGraph
	{
	public:
		RenderGraph(Graphics& graphics);
		virtual ~RenderGraph() = default;

		void Execute(Graphics& graphics);
		void Reset();

		RenderQueuePass& GetRenderQueue(const std::string& passName);

	protected:
		void finalize();

		void addGlobalSource(std::unique_ptr<Source>);
		void addGlobalSink(std::unique_ptr<Sink>);
		void appendPass(std::unique_ptr<Pass> pass);

		void setSinkTarget(const std::string& sinkName, const std::string& target);

	private:
		void linkSinks(Pass& pass);
		void linkGlobalSinks();

	private:
		std::vector<std::unique_ptr<Pass>> mPasses;
		std::vector<std::unique_ptr<Source>> mGlobalSources;
		std::vector<std::unique_ptr<Sink>> mGlobalSinks;
		std::shared_ptr<Bind::RenderTarget> mBackBufferTarget;
		std::shared_ptr<Bind::DepthStencil> mMasterDepth;
		bool mbIsFinalized = false;
	};
}