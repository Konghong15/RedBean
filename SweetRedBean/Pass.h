#pragma once

class Graphics;

namespace Bind
{
	class RenderTarget;
	class DepthStencil;
}

namespace Rgph
{
	class Sink;
	class Source;

	class Pass
	{
	public:
		Pass(std::string name);
		virtual ~Pass() = default;

		virtual void Execute(Graphics& graphics) const = 0;
		virtual void Reset();
		virtual void Finalize();

		void SetSinkLinkage(const std::string& registeredName, const std::string& target);

		const std::string& GetName() const;
		const std::vector<std::unique_ptr<Sink>>& GetSinks() const;
		Source& GetSource(const std::string& registeredName) const;
		Sink& GetSink(const std::string& registeredName) const;

	protected:
		void registerSink(std::unique_ptr<Sink> sink);
		void registerSource(std::unique_ptr<Source> source);

	private:
		std::vector<std::unique_ptr<Sink>> mSinks;
		std::vector<std::unique_ptr<Source>> mSources;
		std::string mName;
	};
}
