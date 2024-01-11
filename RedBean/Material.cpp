#include "pch.h"

#include "Material.h"
#include "Texture.h"

Material::Material(aiMaterial* material)
{
	std::filesystem::path basePath = std::filesystem::current_path() / "textures";

	auto addTexture = [&](aiTextureType aiTextureType, eTexutreType textureType)
		{
			aiString texturePath;

			if (material->GetTexture(aiTextureType, 0, &texturePath) == AI_SUCCESS)
			{
				std::filesystem::path filePath = common::D3DHelper::ConvertStrToWStr(std::string(texturePath.C_Str()));

				auto curPath = basePath / filePath.filename();

				// resourcemanager ;
				// mTextures[static_cast<size_t>(textureType)] = ;
			}
		};

	addTexture(aiTextureType_DIFFUSE, eTexutreType::Diffuse);
	addTexture(aiTextureType_SPECULAR, eTexutreType::Diffuse);
	addTexture(aiTextureType_NORMALS, eTexutreType::Normal);
	addTexture(aiTextureType_EMISSIVE, eTexutreType::Emissive);
	addTexture(aiTextureType_OPACITY, eTexutreType::Opacity);
	addTexture(aiTextureType_METALNESS, eTexutreType::Metalness);
	addTexture(aiTextureType_SHININESS, eTexutreType::Shininess);
}