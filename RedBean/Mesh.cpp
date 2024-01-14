#include "pch.h"

#include "Mesh.h"

namespace resource
{
	// 이 함수는 외부에서 사용하지 못함
	static DirectX::SimpleMath::Matrix convertMatrix(const aiMatrix4x4& aiMatrix)
	{
		DirectX::SimpleMath::Matrix result;

		for (int i = 0; i < 4; ++i)
		{
			result.m[i][0] = aiMatrix[i][0];
			result.m[i][1] = aiMatrix[i][1];
			result.m[i][2] = aiMatrix[i][2];
			result.m[i][3] = aiMatrix[i][3];
		}

		return result;
	}

	Mesh::Mesh(ID3D11Device* device, aiMesh* mesh)
		: mName(mesh->mName.C_Str())
	{
		for (UINT j = 0; j < mesh->mNumVertices; ++j)
		{
			builtIn::vertex::PosTexNormalTan vertex;

			if (mesh->HasPositions())
			{
				vertex.Position.x = mesh->mVertices[j].x;
				vertex.Position.y = mesh->mVertices[j].y;
				vertex.Position.z = mesh->mVertices[j].z;
			}

			if (mesh->HasNormals())
			{
				vertex.Normal.x = mesh->mNormals[j].x;
				vertex.Normal.y = mesh->mNormals[j].y;
				vertex.Normal.z = mesh->mNormals[j].z;
			}

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.Tangent.x = mesh->mTangents[j].x;
				vertex.Tangent.y = mesh->mTangents[j].y;
				vertex.Tangent.z = mesh->mTangents[j].z;
			}

			if (mesh->HasTextureCoords(0))
			{
				vertex.UV.x = (float)mesh->mTextureCoords[0][j].x;
				vertex.UV.y = (float)mesh->mTextureCoords[0][j].y;
			}

			mVertices.push_back(vertex);
		}

		for (UINT j = 0; j < mesh->mNumFaces; ++j) {
			aiFace face = mesh->mFaces[j];

			for (UINT k = 0; k < face.mNumIndices; ++k)
			{
				mIndices.push_back(face.mIndices[k]);
			}
		}

		mVB.Init(device, mVertices);
		mIB.Init(device, mIndices);
	}

	bool MeshResource::Init(ID3D11Device* device, string filename)
	{
		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
		unsigned int importFlags = aiProcess_Triangulate |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_CalcTangentSpace |
			aiProcess_LimitBoneWeights |
			aiProcess_ConvertToLeftHanded;

		const aiScene* scene = importer.ReadFile(filename, importFlags);

		if (scene == nullptr)
		{
			return false;
		}

		mFileName = filename;

		mMeshes.clear();
		mMeshes.reserve(scene->mNumMeshes);

		function<void(aiNode*)> nodeRecursive = [&](aiNode* node)
		{
			for (UINT i = 0; i < node->mNumMeshes; ++i)
			{
				unsigned int meshIndex = node->mMeshes[i];
				aiMesh* mesh = scene->mMeshes[meshIndex];

				mMeshes.emplace_back(device, mesh);
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

	SkinnedMesh::SkinnedMesh(ID3D11Device* device, aiMesh* mesh)
		: mName(mesh->mName.C_Str())
	{
		for (UINT j = 0; j < mesh->mNumVertices; ++j)
		{
			builtIn::vertex::PosTexNormalTanSkinned vertex;

			if (mesh->HasPositions())
			{
				vertex.Position.x = mesh->mVertices[j].x;
				vertex.Position.y = mesh->mVertices[j].y;
				vertex.Position.z = mesh->mVertices[j].z;
			}

			if (mesh->HasNormals())
			{
				vertex.Normal.x = mesh->mNormals[j].x;
				vertex.Normal.y = mesh->mNormals[j].y;
				vertex.Normal.z = mesh->mNormals[j].z;
			}

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.Tangent.x = mesh->mTangents[j].x;
				vertex.Tangent.y = mesh->mTangents[j].y;
				vertex.Tangent.z = mesh->mTangents[j].z;
			}

			if (mesh->HasTextureCoords(0))
			{
				vertex.UV.x = (float)mesh->mTextureCoords[0][j].x;
				vertex.UV.y = (float)mesh->mTextureCoords[0][j].y;
			}

			mVertices.push_back(vertex);
		}

		for (UINT j = 0; j < mesh->mNumFaces; ++j) {
			aiFace face = mesh->mFaces[j];

			for (UINT k = 0; k < face.mNumIndices; ++k)
			{
				mIndices.push_back(face.mIndices[k]);
			}
		}

		for (UINT i = 0; i < mesh->mNumBones; ++i)
		{
			aiBone* curBone = mesh->mBones[i];

			Bone bone;
			bone.Name = curBone->mName.C_Str();
			bone.OffsetMatrix = convertMatrix(curBone->mOffsetMatrix).Transpose();
			bone.Index = mBones.size();

			mBones.push_back(bone);

			for (UINT j = 0; j < curBone->mNumWeights; ++j)
			{
				aiVertexWeight curVertexWeight = curBone->mWeights[j];

				for (UINT k = 0; k < 4; ++k)
				{
					if (mVertices[curVertexWeight.mVertexId].Indices[k] == builtIn::vertex::PosTexNormalTanSkinned::INVALID_INDEX)
					{
						mVertices[curVertexWeight.mVertexId].Indices[k] = i;
						mVertices[curVertexWeight.mVertexId].Weights[k] = curVertexWeight.mWeight;
						break;
					}
				}
			}
		}

		mVB.Init(device, mVertices);
		mIB.Init(device, mIndices);
	}

	bool SkinnedMeshResource::Init(ID3D11Device* device, string filename)
	{
		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
		unsigned int importFlags = aiProcess_Triangulate |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_CalcTangentSpace |
			aiProcess_LimitBoneWeights |
			aiProcess_ConvertToLeftHanded;

		const aiScene* scene = importer.ReadFile(filename, importFlags);

		if (scene == nullptr)
		{
			return false;
		}

		mFileName = filename;

		mSkinnedMeshes.clear();
		mSkinnedMeshes.reserve(scene->mNumMeshes);

		function<void(aiNode*)> nodeRecursive = [&](aiNode* node)
		{
			for (UINT i = 0; i < node->mNumMeshes; ++i)
			{
				unsigned int meshIndex = node->mMeshes[i];
				aiMesh* mesh = scene->mMeshes[meshIndex];

				mSkinnedMeshes.emplace_back(device, mesh);
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