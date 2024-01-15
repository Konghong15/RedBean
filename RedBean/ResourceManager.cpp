#include "pch.h"

#include "ResourceManager.h"

#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "Model.h"

namespace renderSystem
{
	directXWrapper::Texture* ResourceManager::CreateTextureOrNull(const string& filename)
	{
		using namespace directXWrapper;

		auto find = mTextures.find(filename);

		if (find != mTextures.end())
		{
			return find->second;
		}

		Texture* texture = new Texture();

		if (!texture->Init(mDevice, common::D3DHelper::ConvertStrToWStr(filename)))
		{
			delete texture;
			return nullptr;
		}

		mTextures.insert({ filename, texture });

		return texture;
	}

	directXWrapper::Texture* ResourceManager::CreateTextureOrNull(const wstring& filename)
	{
		return CreateTextureOrNull(common::D3DHelper::ConvertWStrToStr(filename));
	}

	resource::MeshResource* ResourceManager::CreateMeshResourceOrNull(const string& filename)
	{
		using namespace directXWrapper;
		using namespace resource;

		auto find = mMeshResources.find(filename);

		if (find != mMeshResources.end())
		{
			return find->second;
		}

		MeshResource* meshResource = new MeshResource();

		if (!meshResource->Init(mDevice, filename))
		{
			delete meshResource;
			return nullptr;
		}

		mMeshResources.insert({ filename, meshResource });

		return meshResource;
	}

	resource::MeshResource* ResourceManager::CreateMeshResourceOrNull(const wstring& filename)
	{
		return CreateMeshResourceOrNull(common::D3DHelper::ConvertWStrToStr(filename));
	}
	resource::SkinnedMeshResource* ResourceManager::CreateSkinnedMeshResourceOrNull(const string& filename)
	{
		using namespace directXWrapper;
		using namespace resource;

		auto find = mSkinnedMeshResources.find(filename);

		if (find != mSkinnedMeshResources.end())
		{
			return find->second;
		}

		SkinnedMeshResource* skinnedMeshResource = new SkinnedMeshResource();

		if (!skinnedMeshResource->Init(mDevice, filename))
		{
			delete skinnedMeshResource;
			return nullptr;
		}

		mSkinnedMeshResources.insert({ filename, skinnedMeshResource });

		return skinnedMeshResource;
	}
	resource::SkinnedMeshResource* ResourceManager::CreateSkinnedMeshResourceOrNull(const wstring& filename)
	{
		return CreateSkinnedMeshResourceOrNull(common::D3DHelper::ConvertWStrToStr(filename));;
	}
	resource::MaterialResource* ResourceManager::CreateMaterialResourceOrNull(const string& filename)
	{
		using namespace directXWrapper;
		using namespace resource;

		auto find = mMaterialResources.find(filename);

		if (find != mMaterialResources.end())
		{
			return find->second;
		}

		MaterialResource* materialResource = new MaterialResource();

		if (!materialResource->Init(filename))
		{
			delete materialResource;
			return nullptr;
		}

		mMaterialResources.insert({ filename, materialResource });

		return materialResource;
	}
	resource::MaterialResource* ResourceManager::CreateMaterialResourceOrNull(const wstring& filename)
	{
		return CreateMaterialResourceOrNull(common::D3DHelper::ConvertWStrToStr(filename));;
	}
	resource::AnimationResource* ResourceManager::CreateAnimationResourceOrNull(const string& filename)
	{
		using namespace directXWrapper;
		using namespace resource;

		auto find = mAnimationResources.find(filename);

		if (find != mAnimationResources.end())
		{
			return find->second;
		}

		AnimationResource* animationResource = new AnimationResource();

		if (!animationResource->Init(filename))
		{
			delete animationResource;
			return nullptr;
		}

		mAnimationResources.insert({ filename, animationResource });

		return animationResource;
	}
	resource::AnimationResource* ResourceManager::CreateAnimationResourceOrNull(const wstring& filename)
	{
		return CreateAnimationResourceOrNull(common::D3DHelper::ConvertWStrToStr(filename));;
	}

	resource::Model* ResourceManager::CreateModelOrNull(const string& key, const ModelDesc& modelDesc)
	{
		using namespace directXWrapper;
		using namespace resource;

		auto find = mModels.find(key);

		if (find != mModels.end())
		{
			return find->second;
		}

		MeshResource* meshResource = CreateMeshResourceOrNull(modelDesc.MeshFilePath);
		MaterialResource* materialResource = CreateMaterialResourceOrNull(modelDesc.MaterialFilePath);

		if (meshResource == nullptr
			|| materialResource == nullptr)
		{
			return nullptr;
		}

		Model* model = new Model();

		if (!model->Init(meshResource, materialResource))
		{
			delete model;
			return nullptr;
		}

		mModels.insert({ key, model });

		return model;
	}
	resource::Model* ResourceManager::CreateModelOrNull(const string& key, const string& filename)
	{
		return CreateModelOrNull(key, { filename, filename });
	}

	resource::SkinnedModel* ResourceManager::CreateSkinnedModelOrNull(const string& key, const SkinnedModelDesc& skinnedModelDsec)
	{
		using namespace directXWrapper;
		using namespace resource;

		auto find = mSkinnedModels.find(key);

		if (find != mSkinnedModels.end())
		{
			return find->second;
		}

		SkinnedMeshResource* meshResource = CreateSkinnedMeshResourceOrNull(skinnedModelDsec.MeshFilePath);
		MaterialResource* materialResource = CreateMaterialResourceOrNull(skinnedModelDsec.MaterialFilePath);
		AnimationResource* animationResource = CreateAnimationResourceOrNull(skinnedModelDsec.MaterialFilePath);

		if (meshResource == nullptr
			|| materialResource == nullptr
			|| animationResource == nullptr)
		{
			return nullptr;
		}

		SkinnedModel* skinnedModel = new SkinnedModel();

		if (!skinnedModel->Init(meshResource, materialResource, animationResource))
		{
			delete skinnedModel;
			return nullptr;
		}

		mSkinnedModels.insert({ key, skinnedModel });

		return skinnedModel;
	}
	resource::SkinnedModel* ResourceManager::CreateSkinnedModelOrNull(const string& key, const string& filename)
	{
		return CreateSkinnedModelOrNull(key, { filename, filename, filename });
	}

	resource::Model* ResourceManager::GetModelOrNull(const string& key)
	{
		auto find = mModels.find(key);

		if (find == mModels.end())
		{
			return nullptr;
		}

		return find->second;
	}
	resource::SkinnedModel* ResourceManager::GetSkinnedModelOrNull(const string& key)
	{
		auto find = mSkinnedModels.find(key);

		if (find == mSkinnedModels.end())
		{
			return nullptr;
		}

		return find->second;
	}
}