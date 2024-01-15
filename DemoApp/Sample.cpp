#include "pch.h"

#include "Sample.h"
#include "../RedBean/Animation.h"

namespace entryPoint
{
	Sample::Sample(HINSTANCE hInstance, UINT width, UINT height, std::wstring name)
		: Processor(hInstance, width, height, name)
	{
	}

	bool Sample::Init()
	{
		if (!Processor::Init())
		{
			return false;
		}

		mGraphic.Init(mhWnd, mhInstance, mWidth, mHeight);
		renderSystem::RenderManager::GetInstance()->Init(mGraphic.GetDevice(), mGraphic.GetContext());
		renderSystem::ResourceManager::GetInstance()->Init(mGraphic.GetDevice());

		auto* skinnedModel = renderSystem::ResourceManager::GetInstance()->CreateSkinnedModelOrNull("dancing", "../Resource/Models/dancing.fbx");
		mSkinnedModelInstance.ModelRef = skinnedModel;
		mSkinnedModelInstance.ClipName = skinnedModel->GetAnimationResource()->GetAnimationClips().begin()->first;
		mSkinnedModelInstance.TimePos = 0.f;

		auto* model = renderSystem::ResourceManager::GetInstance()->CreateModelOrNull("dancing", "../Resource/Models/pbrModel.fbx");
		mModelInstance.ModelRef = model;
		mModelInstance.WorldMatrix = Matrix::CreateTranslation({ 0, 10, 10 });

		// renderSystem::RenderManager::GetInstance()->Register(&mSkinnedModelInstance);
		renderSystem::RenderManager::GetInstance()->Register(&mModelInstance);

		renderSystem::RenderManager::GetInstance()->GetCamera().SetLens(0.25f * MathHelper::Pi, GetAspectRatio(), 1.0f, 10000.0f);
		renderSystem::RenderManager::GetInstance()->GetCamera().SetPosition({ 0, 100, -500 });

		return true;
	}

	void Sample::Update(float deltaTime)
	{
		if (GetAsyncKeyState('W') & 0x8000)
		{
			renderSystem::RenderManager::GetInstance()->GetCamera().TranslateLook(100 * deltaTime);
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			renderSystem::RenderManager::GetInstance()->GetCamera().TranslateLook(-100 * deltaTime);
		}

		mSkinnedModelInstance.TimePos += deltaTime;
		mSkinnedModelInstance.TimePos = fmod(mSkinnedModelInstance.TimePos, mSkinnedModelInstance.ModelRef->GetAnimationResource()->GetAnimationClips().begin()->second.mDuration);

		if (GetAsyncKeyState('Q') & 0x8000)
		{
			mModelInstance.WorldMatrix *= Matrix::CreateTranslation({ 0, 0, deltaTime * 100 });
			mSkinnedModelInstance.WorldMatrix *= Matrix::CreateTranslation({ 0, 0, deltaTime * 100 });
		}
	}

	void Sample::Render()
	{
		mGraphic.BeginRender();

		renderSystem::RenderManager::GetInstance()->RenderAll();

		mGraphic.EndRender();
	}
}