#pragma once

#include "../Processor/Processor.h"

namespace entryPoint
{
	using namespace common;

	class Sample final : public Processor
	{
	public:
		Sample(HINSTANCE hInstance, UINT width, UINT height, std::wstring name);

		bool Init() override;
		void Update(float deltaTime) override;
		void Render() override;

	private:
	};
}