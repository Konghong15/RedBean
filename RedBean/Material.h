#pragma once

namespace directXWraper
{
	class Texture;
	class ShaderProgram;
}

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
	using TextureArray = std::array<shared_ptr<directXWraper::Texture>, static_cast<size_t>(eTexutreType::Size)>;

public:
	Material(aiMaterial* material);
	~Material() = default;

	shared_ptr<directXWraper::Texture> GetTextureOrNull(eTexutreType textureType) { return mTextures[static_cast<size_t>(textureType)]; }
	const TextureArray& GetTextures() { return mTextures; }
	bool GetIsPBRTexture() const { return mTextures[(size_t)eTexutreType::Metalness] != nullptr || mTextures[(size_t)eTexutreType::Shininess] != nullptr; }

private:
	string Name;
	common::Material mMaterialFacter;
	TextureArray mTextures;
};

