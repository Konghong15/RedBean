#include "pch.h"

#include "Material.h"
#include "Texture.h"
#include "ResourceManager.h"

namespace resource
{
	Material::Material(aiMaterial* material, const filesystem::path& basePath)
		: mTextures{ nullptr, }
		, mName(material->GetName().C_Str())
		, mbUseBlend(false)
	{
		auto addTexture = [&](aiTextureType aiTextureType, eTexutreType textureType) -> bool
			{
				aiString texturePath;

				if (material->GetTexture(aiTextureType, 0, &texturePath) == AI_SUCCESS)
				{
					std::filesystem::path filePath = common::D3DHelper::ConvertStrToWStr(std::string(texturePath.C_Str()));

					auto curPath = basePath / filePath.filename();

					mTextures[static_cast<size_t>(textureType)] = renderSystem::ResourceManager::GetInstance()->CreateTextureOrNull(curPath.string());

					return true;
				}

				return false;
			};

		if (!addTexture(aiTextureType_DIFFUSE, eTexutreType::Diffuse))
		{
			addTexture(aiTextureType_BASE_COLOR, eTexutreType::Diffuse);
		}
		addTexture(aiTextureType_SPECULAR, eTexutreType::Specular);
		addTexture(aiTextureType_NORMALS, eTexutreType::Normal);
		addTexture(aiTextureType_EMISSIVE, eTexutreType::Emissive);
		if (addTexture(aiTextureType_OPACITY, eTexutreType::Opacity))
		{
			mName = material->GetName().C_Str();
		}
		addTexture(aiTextureType_METALNESS, eTexutreType::Metalness);
		addTexture(aiTextureType_SHININESS, eTexutreType::Roughness);
	}

	MaterialResource::MaterialResource(const aiScene* scene, const filesystem::path& basePath)
		: mName(scene->mName.C_Str())
	{
		mMaterials.reserve(scene->mNumMeshes);

		function<void(aiNode*)> nodeRecursive = [&](aiNode* node)
			{
				for (UINT i = 0; i < node->mNumMeshes; ++i)
				{
					unsigned int meshIndex = node->mMeshes[i];
					aiMesh* mesh = scene->mMeshes[meshIndex];
					aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

					mMaterials.emplace_back(material, basePath);
				}

				for (UINT i = 0; i < node->mNumChildren; ++i)
				{
					nodeRecursive(node->mChildren[i]);
				}
			};

		nodeRecursive(scene->mRootNode);
	}
}