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

		renderSystem::ResourceManager::SkinnedModelDesc desc;
		desc.NodeFilePath = "../Resource/Models/dancing.fbx";
		desc.MeshFilePath = "../Resource/Models/dancing.fbx";
		desc.MaterialFilePath = "../Resource/Models/dancing.fbx";
		desc.AnimationFilePath = "../Resource/Models/dancing.fbx";

		auto* model = renderSystem::ResourceManager::GetInstance()->CreateSkinnedModelOrNull("dancing", desc);
		mSkinnedModelInstance.ModelRef = model;
		mSkinnedModelInstance.AnimationName = model->GetAnimationResource()->GetAnimationClips().begin()->first;
		mSkinnedModelInstance.TimePos = 0.f;
		mSkinnedModelInstance.WorldMatrix = Matrix::Identity;

		renderSystem::RenderManager::GetInstance()->Register(&mSkinnedModelInstance);

		return true;
	}

	void Sample::Update(float deltaTime)
	{

	}

	void Sample::Render()
	{
		mGraphic.BeginRender();

		renderSystem::RenderManager::GetInstance()->RenderAll();

		mGraphic.EndRender();
	}
}