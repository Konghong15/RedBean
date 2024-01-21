#include "pch.h"

#include "Sample.h"

#include <random>

#include "../RedBean/Animation.h"
#include "../RedBean/Material.h"

namespace entryPoint
{
	Sample::Sample(HINSTANCE hInstance, UINT width, UINT height, std::wstring name)
		: Processor(hInstance, width, height, name)
	{
	}

	Sample::~Sample()
	{
	}

	bool Sample::Init()
	{
		if (!Processor::Init())
		{
			return false;
		}

		mCamera.SetLens(0.25f * MathHelper::Pi, GetAspectRatio(), 1.0f, 10000.0f);

		mSweetGrapic = make_unique<Graphics>(mhWnd, mWidth, mHeight);
		mSweetGrapic->SetProjection(mCamera.GetProj());

		mRenderGraph = std::make_unique<Rgph::BlurOutlineRenderGraph>(*mSweetGrapic);
		mLight = std::make_unique<::PointLight>(*mSweetGrapic);
		mModel = std::make_unique<::Model>(*mSweetGrapic, "../Resource/Models/");

		mLight->LinkTechniques(*mRenderGraph);
		mModel->LinkTechniques(*mRenderGraph);

		return true;
	}

	void Sample::Update(float deltaTime)
	{
		if (mSweetGrapic == nullptr)
		{
			return;
		}

		mSweetGrapic->SetView(mCamera.GetView());
		mLight->Bind(*mSweetGrapic, mCamera.GetView());
	}

	void Sample::Render()
	{
		if (mSweetGrapic == nullptr)
		{
			return;
		}

		mSweetGrapic->BeginFrame();

		mLight->Submit();
		mModel->Submit();

		mRenderGraph->Execute(*mSweetGrapic);

		/*
		임구이처리 프로브 보내서 다루기
		*/

		mSweetGrapic->EndFrame();
	}
}