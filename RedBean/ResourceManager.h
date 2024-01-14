#pragma once

namespace directXWrapper
{
	class Texture;
}

namespace resource
{
	class NodeResource;
	class MeshResource;
	class SkinnedMeshResource;
	class MaterialResource;
	class AnimationResource;
	class Model;
	class SkinnedModel;
}

namespace renderSystem
{
	class ResourceManager : public Singleton<ResourceManager>
	{
		friend class Singleton<ResourceManager>;

	public:
		struct ModelDesc
		{
			string NodeFilePath;
			string MeshFilePath;
			string MaterialFilePath;
		};

		struct SkinnedModelDesc
		{
			string NodeFilePath;
			string MeshFilePath;
			string MaterialFilePath;
			string AnimationFilePath;
		};

	public:
		void Init(ID3D11Device* device) { mDevice = device; }

		directXWrapper::Texture* CreateTextureOrNull(const string& filename);
		directXWrapper::Texture* CreateTextureOrNull(const wstring& filename);
		resource::NodeResource* CreateNodeResourceOrNull(const string& filename);
		resource::NodeResource* CreateNodeResourceOrNull(const wstring& filename);
		resource::MeshResource* CreateMeshResourceOrNull(const string& filename);
		resource::MeshResource* CreateMeshResourceOrNull(const wstring& filename);
		resource::SkinnedMeshResource* CreateSkinnedMeshResourceOrNull(const string& filename);
		resource::SkinnedMeshResource* CreateSkinnedMeshResourceOrNull(const wstring& filename);
		resource::MaterialResource* CreateMaterialResourceOrNull(const string& filename);
		resource::MaterialResource* CreateMaterialResourceOrNull(const wstring& filename);
		resource::AnimationResource* CreateAnimationResourceOrNull(const string& filename);
		resource::AnimationResource* CreateAnimationResourceOrNull(const wstring& filename);
		resource::Model* CreateModelOrNull(const string& key, const ModelDesc& modelDesc);
		resource::SkinnedModel* CreateSkinnedModelOrNull(const string& key, const SkinnedModelDesc& skinnedModelDsec);
		resource::Model* GetModelOrNull(const string& key);
		resource::SkinnedModel* GetSkinnedModelOrNull(const string& key);

	private:
		ResourceManager() = default;
		~ResourceManager() = default;

	private:
		ID3D11Device* mDevice;
		map<string, directXWrapper::Texture*> mTextures;
		map<string, resource::NodeResource*> mNodeResources;
		map<string, resource::MeshResource*> mMeshResources;
		map<string, resource::SkinnedMeshResource*> mSkinnedMeshResources;
		map<string, resource::MaterialResource*> mMaterialResources;
		map<string, resource::AnimationResource*> mAnimationResources;
		map<string, resource::Model*> mModels;
		map<string, resource::SkinnedModel*> mSkinnedModels;
	};
}
