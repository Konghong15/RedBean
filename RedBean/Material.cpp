#include "pch.h"

#include "Material.h"
#include "Texture.h"
#include "ResourceManager.h"

namespace resource
{
	Material::Material(aiMaterial* material, filesystem::path basePath)
		: mTextures{ nullptr, }
	{
		auto addTexture = [&](aiTextureType aiTextureType, eTexutreType textureType)
		{
			aiString texturePath;

			if (material->GetTexture(aiTextureType, 0, &texturePath) == AI_SUCCESS)
			{
				std::filesystem::path filePath = common::D3DHelper::ConvertStrToWStr(std::string(texturePath.C_Str()));

				auto curPath = basePath / filePath.filename();

				mTextures[static_cast<size_t>(textureType)] = renderSystem::ResourceManager::GetInstance()->CreateTextureOrNull(curPath.string());
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

	bool MaterialResource::Init(string filename)
	{
		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
		unsigned int importFlags = 0;

		const aiScene* scene = importer.ReadFile(filename, importFlags);

		if (scene == nullptr)
		{
			return false;
		}

		mFilename = filename;
		mMaterials.clear();
		mMaterials.reserve(scene->mNumMeshes);

		function<void(aiNode*)> nodeRecursive = [&](aiNode* node)
		{
			for (UINT i = 0; i < node->mNumMeshes; ++i)
			{
				unsigned int meshIndex = node->mMeshes[i];
				aiMesh* mesh = scene->mMeshes[meshIndex];
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				mMaterials.emplace_back(material);
			}

			for (UINT i = 0; i < node->mNumChildren; ++i)
			{
				nodeRecursive(node->mChildren[i]);
			}
		};

		nodeRecursive(scene->mRootNode);
		importer.FreeScene();

		return true;
	}
}