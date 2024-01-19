#pragma once

namespace resource
{
	enum class eTexutreType
	{
		Diffuse,
		Specular,
		Normal,
		Opacity,
		Emissive,
		Metalness,
		Roughness,
		Size,
	};
	enum
	{
		eTextureTypeSize = static_cast<size_t>(eTexutreType::Size)
	};

	class Texture;

	class Material
	{
	public:
		Material(aiMaterial* material, const filesystem::path& basePath);
		~Material() = default;

		void SetUseBlend(bool bUseBlend) { mbUseBlend = bUseBlend; }

		bool GetHasPBRTexture() const { return mTextures[(size_t)eTexutreType::Metalness] != nullptr || mTextures[(size_t)eTexutreType::Roughness] != nullptr; }
		bool GetHasAlpha() const { return mTextures[(size_t)eTexutreType::Opacity] != nullptr; }
		Texture* GetTextureOrNull(eTexutreType textureType) const { return mTextures[static_cast<size_t>(textureType)]; }

		const string& GetName() const { return mName; }
		const array<Texture*, eTextureTypeSize>& GetTextures() const { return mTextures; }

	private:
		string mName;
		array<Texture*, eTextureTypeSize> mTextures;
		bool mbUseBlend;
	};

	class MaterialResource
	{
	public:
		MaterialResource(const aiScene* scene, const filesystem::path& basePath);
		~MaterialResource() = default;

		const Material& GetMaterial() const;
		bool GetMaterial(string name, Material* materia) const;

		const string& GetName() const { return mName; }
		const vector<Material>& GetMaterials() const { return mMaterials; }

	private:
		string mName;
		vector<Material> mMaterials;
	};
}