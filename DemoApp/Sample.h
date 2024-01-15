#pragma once

#include "../Processor/Processor.h"
#include "../RedBean/Graphic.h"
#include "../RedBean/RenderManager.h"
#include "../RedBean/ResourceManager.h"
#include "../RedBean/Model.h"

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
		renderSystem::Graphic mGraphic;
		resource::SkinnedModelInstance mSkinnedModelInstance;
		resource::ModelInstance mModelInstance;
	};
}