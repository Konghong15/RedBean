#pragma once


namespace directXWrapper
{
	class Texture;
	class ShaderProgram;
}

namespace resource
{
	enum class eTexutreType
	{
		Diffuse,
		Specular,
		Normal,
		Emissive,
		Opacity,
		Metalness,
		Shininess, // smoothness
		Size,
	};

	class Material
	{
	public:
		using TextureArray = std::array<directXWrapper::Texture*, static_cast<size_t>(eTexutreType::Size)>;

	public:
		Material(aiMaterial* material, std::filesystem::path basePath = filesystem::current_path() / "../Resource/textures");
		~Material() = default;

		directXWrapper::Texture* GetTextureOrNull(eTexutreType textureType) const { return mTextures[static_cast<size_t>(textureType)]; }
		const TextureArray& GetTextures() const { return mTextures; }
		bool GetIsPBRTexture() const { return mTextures[(size_t)eTexutreType::Metalness] != nullptr || mTextures[(size_t)eTexutreType::Shininess] != nullptr; }

	private:
		string Name;
		TextureArray mTextures;
	};

	class MaterialResource
	{
	public:
		MaterialResource() = default;
		~MaterialResource() = default;

		bool Init(string filename);

		const string& GetFileName() const { return mFilename; }
		const vector<Material>& GetMaterials() const { return mMaterials; }

	private:
		string mFilename;
		vector<Material> mMaterials;
	};
}