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
	Material(aiMaterial* material);
	~Material() = default;

	shared_ptr<directXWraper::Texture> GetTextureOrNull(eTexutreType textureType) { return mTextures[static_cast<size_t>(textureType)]; }
	bool GetIsPBRTexture() const { return mbIsPBRTexture; }

private:
	string Name;
	bool mbIsPBRTexture;
	std::array<shared_ptr<directXWraper::Texture>, static_cast<size_t>(eTexutreType::Size)> mTextures;
};

