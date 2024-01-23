#include "pch.h"

#include "Sample.h"

#include <random>

#include "../SweetRedBean/BindableResourceManager.h"
#include "../RedBean/Model.h"
#include "../RedBean/ResourceManager.h"
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
		mCamera.SetPosition(0, 0, -100);

		mSweetGrapic = make_unique<Graphics>(mhWnd, mWidth, mHeight);
		mSweetGrapic->SetProjection(mCamera.GetProj());

		mRenderGraph = std::make_unique<Rgph::BlurOutlineRenderGraph>(*mSweetGrapic);

		mModel = std::make_unique<::Model>(*mSweetGrapic, "../Resource/Models/pbrModel.fbx");
		mFrameLight = std::make_unique<::FrameLight>(*mSweetGrapic);

		mModel->LinkTechniques(*mRenderGraph);
		mFrameLight->LinkTechnique(*mRenderGraph);

		renderSystem::ResourceManager::GetInstance()->Init(mSweetGrapic->GetDevice().Get());
		renderSystem::ResourceManager::GetInstance()->CreateModelOrNull("coco", "../Resource/Models/pbrModel.fbx");
			Bind::ResourceManager::Get();

		return true;
	}

	void Sample::Update(float deltaTime)
	{
		if (mSweetGrapic == nullptr)
		{
			return;
		}
		if (GetAsyncKeyState('W') & 0x8000)
		{
			mCamera.TranslateLook(deltaTime * 100);
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			mCamera.TranslateLook(-deltaTime * 100);
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			mCamera.TranslateRight(-deltaTime * 100);
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			mCamera.TranslateRight(deltaTime * 100);
		}
		if (GetAsyncKeyState('Q') & 0x8000)
		{
			mCamera.TranslateUp(deltaTime * 100);
		}
		if (GetAsyncKeyState('E') & 0x8000)
		{
			mCamera.TranslateUp(-deltaTime * 100);
		}

		mCamera.UpdateViewMatrix();
	}

	void Sample::Render()
	{
		if (mSweetGrapic == nullptr)
		{
			return;
		}

		// 상수 버퍼 관리
		{
			mSweetGrapic->SetView(mCamera.GetView());
			mFrameLight->Bind(*mSweetGrapic, mCamera.GetPosition());
		}

		mSweetGrapic->BeginFrame();

		// mLight->Submit();
		mModel->Submit();

		mRenderGraph->Execute(*mSweetGrapic);

		/*
		임구이처리 프로브 보내서 다루기
		*/
		mFrameLight->SpawnControlWindow();

		mSweetGrapic->EndFrame();

		mRenderGraph->Reset();
	}
}