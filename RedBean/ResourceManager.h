#pragma once

namespace resource
{
	class Texture;
	class MeshResource;
	class SkinnedMeshResource;
	class MaterialResource;
	class AnimationResource;
	class Model;
	class SkinnedModel;
}

namespace renderSystem
{
	struct ModelDesc
	{
		string MeshFilePath;
		string MaterialFilePath;
	};

	struct SkinnedModelDesc
	{
		string MeshFilePath;
		string MaterialFilePath;
		string AnimationFilePath;
	};

	class ResourceManager : public Singleton<ResourceManager>
	{
		friend class Singleton<ResourceManager>;

	public:
		void Init(ID3D11Device* device) { mDevice = device; }
		void Destroy();

		resource::Texture* CreateTextureOrNull(const string& filename);
		resource::Texture* CreateTextureOrNull(const wstring& filename);

		resource::MeshResource* CreateMeshResourceOrNull(const string& filename);
		resource::MeshResource* CreateMeshResourceOrNull(const wstring& filename);

		resource::SkinnedMeshResource* CreateSkinnedMeshResourceOrNull(const string& filename);
		resource::SkinnedMeshResource* CreateSkinnedMeshResourceOrNull(const wstring& filename);

		resource::MaterialResource* CreateMaterialResourceOrNull(const string& filename, const std::filesystem::path& basePath = filesystem::current_path() / "../Resource/textures");
		resource::MaterialResource* CreateMaterialResourceOrNull(const wstring& filename, const std::filesystem::path& basePath = filesystem::current_path() / "../Resource/textures");

		resource::AnimationResource* CreateAnimationResourceOrNull(const string& filename);
		resource::AnimationResource* CreateAnimationResourceOrNull(const wstring& filename);

		resource::Model* CreateModelOrNull(const string& key, const string& filename);
		resource::Model* CreateModelOrNull(const string& key, const ModelDesc& modelDesc);

		resource::SkinnedModel* CreateSkinnedModelOrNull(const string& key, const string& filename);
		resource::SkinnedModel* CreateSkinnedModelOrNull(const string& key, const SkinnedModelDesc& skinnedModelDsec);

		resource::Model* GetModelOrNull(const string& key);
		resource::SkinnedModel* GetSkinnedModelOrNull(const string& key);

	private:
		ResourceManager() = default;
		~ResourceManager() { Destroy(); }
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

	private:
		ID3D11Device* mDevice;
		map<string, resource::Texture*> mTextures;
		map<string, resource::MeshResource*> mMeshResources;
		map<string, resource::SkinnedMeshResource*> mSkinnedMeshResources;
		map<string, resource::MaterialResource*> mMaterialResources;
		map<string, resource::AnimationResource*> mAnimationResources;
		map<string, resource::Model*> mModels;
		map<string, resource::SkinnedModel*> mSkinnedModels;
	};
}
