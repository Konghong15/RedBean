#pragma once

#include "../Processor/Processor.h"
#include "../RedBean/Camera.h"

#include "../SweetRedBean/Graphics.h"
#include "../SweetRedBean/IDrawable.h"
#include "../SweetRedBean/PointLight.h"
#include "../SweetRedBean/Model.h"
#include "../SweetRedBean/BlurOutlineRenderGraph.h"

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

		vector<unique_ptr<IDrawable>> mDrawables;

		unique_ptr<Graphics> mSweetGrapic;
		unique_ptr<Rgph::BlurOutlineRenderGraph> mRenderGraph;
		unique_ptr<::PointLight> mLight;
		unique_ptr<::Model> mModel;
	};
}