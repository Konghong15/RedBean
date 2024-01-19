#pragma once

#include "../Processor/Processor.h"
#include "../RedBean/Renderer.h"
#include "../RedBean/RenderManager.h"
#include "../RedBean/ResourceManager.h"
#include "../RedBean/Model.h"
#include "../RedBean/Camera.h"
#include "../SweetRedBean/Graphics.h"
#include "../SweetRedBean/IDrawable.h"

namespace entryPoint
{
	using namespace common;

	class Sample final : public Processor
	{
	public:
		Sample(HINSTANCE hInstance, UINT width, UINT height, std::wstring name);
		virtual ~Sample() override;

		bool Init() override;
		void Update(float deltaTime) override;
		void Render() override;

	private:
		common::Camera mCamera;

		unique_ptr<Graphics> mSweetGrapic;
		vector<unique_ptr<IDrawable>> mDrawables;
	};
}