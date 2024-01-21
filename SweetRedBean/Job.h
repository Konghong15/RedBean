#pragma once

class Graphics;
class IDrawable;
class Step;

namespace Rgph
{
	class Job
	{
	public:
		Job(const Step* pStep, const IDrawable* pDrawable);
		~Job() = default;

		void Execute(Graphics& graphics) const;

	private:
		const IDrawable* mpDrawable;
		const Step* mpStep;
	};
}