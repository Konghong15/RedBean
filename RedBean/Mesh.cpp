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

		function<void(aiNode*, Node*)> nodeRecursive = [&](aiNode* node, Node* myNodeParent)
		{
			Node* myNode = new Node();
			myNode->Name = node->mName.C_Str();
			myNode->Index = mNodes.size();
			myNode->ToParentMatrix = convertMatrix(node->mTransformation).Transpose();

			if (myNodeParent != nullptr)
			{
				myNode->ParentIndex = myNodeParent->Index;
				myNode->ToRootMatrix = myNode->ToParentMatrix * myNodeParent->ToRootMatrix;

				myNode->Parent = myNodeParent;
				myNodeParent->Children.push_back(myNode);
			}
			else
			{
				myNode->ParentIndex = Node::INVALID_INDEX;
				myNode->Parent = nullptr;

				myNode->ToRootMatrix = myNode->ToParentMatrix;
			}

			mNodes.push_back(myNode);
			myNode->ContainMeshesIndex.reserve(node->mNumMeshes);

			for (UINT i = 0; i < node->mNumMeshes; ++i)
			{
				unsigned int meshIndex = node->mMeshes[i];
				aiMesh* mesh = scene->mMeshes[meshIndex];

				myNode->ContainMeshesIndex.push_back(mMeshes.size());
				mMeshes.emplace_back(device, mesh);
			}

			for (UINT i = 0; i < node->mNumChildren; ++i)
			{
				nodeRecursive(node->mChildren[i], myNode);
			}
		};

		nodeRecursive(scene->mRootNode, nullptr);
		importer.FreeScene();

		return true;
	}

	SkinnedMesh::SkinnedMesh(ID3D11Device* device, aiMesh* mesh)
		: mName(mesh->mName.C_Str())
	{
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			builtIn::vertex::PosTexNormalTanSkinned vertex;

			if (mesh->HasPositions())
			{
				vertex.Position.x = mesh->mVertices[i].x;
				vertex.Position.y = mesh->mVertices[i].y;
				vertex.Position.z = mesh->mVertices[i].z;
			}

			if (mesh->HasNormals())
			{
				vertex.Normal.x = mesh->mNormals[i].x;
				vertex.Normal.y = mesh->mNormals[i].y;
				vertex.Normal.z = mesh->mNormals[i].z;
			}

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.Tangent.x = mesh->mTangents[i].x;
				vertex.Tangent.y = mesh->mTangents[i].y;
				vertex.Tangent.z = mesh->mTangents[i].z;
			}

			if (mesh->HasTextureCoords(0))
			{
				vertex.UV.x = (float)mesh->mTextureCoords[0][i].x;
				vertex.UV.y = (float)mesh->mTextureCoords[0][i].y;
			}

			mVertices.push_back(vertex);
		}

		for (UINT j = 0; j < mesh->mNumFaces; ++j) {
			const aiFace& face = mesh->mFaces[j];

			for (UINT k = 0; k < face.mNumIndices; ++k)
			{
				mIndices.push_back(face.mIndices[k]);
			}
		}

		mBones.reserve(mesh->mNumBones);

		for (UINT i = 0; i < mesh->mNumBones; ++i)
		{
			aiBone* curBone = mesh->mBones[i];

			Bone bone;
			bone.Name = curBone->mName.C_Str();
			bone.OffsetMatrix = convertMatrix(curBone->mOffsetMatrix).Transpose();

			mBones.push_back(bone);

			for (UINT j = 0; j < curBone->mNumWeights; ++j)
			{
				aiVertexWeight curVertexWeight = curBone->mWeights[j];

				unsigned int vertexID = curVertexWeight.mVertexId;
				float vertexWeight = curVertexWeight.mWeight;

				for (UINT k = 0; k < 4; ++k)
				{
					if (mVertices[vertexID].Indices[k] == builtIn::vertex::PosTexNormalTanSkinned::INVALID_INDEX)
					{
						mVertices[vertexID].Indices[k] = i;
						mVertices[vertexID].Weights[k] = vertexWeight;
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
		mNodes.reserve(128);

		function<void(aiNode*, Node*)> nodeRecursive = [&](aiNode* node, Node* myNodeParent)
		{
			Node* myNode = new Node();
			myNode->Name = node->mName.C_Str();
			myNode->Index = mNodes.size();
			myNode->ToParentMatrix = convertMatrix(node->mTransformation).Transpose();

			if (myNodeParent != nullptr)
			{
				myNode->ParentIndex = myNodeParent->Index;
				myNode->ToRootMatrix = myNode->ToParentMatrix * myNodeParent->ToRootMatrix;

				myNode->Parent = myNodeParent;
				myNodeParent->Children.push_back(myNode);
			}
			else
			{
				myNode->ParentIndex = Node::INVALID_INDEX;
				myNode->Parent = nullptr;

				myNode->ToRootMatrix = myNode->ToParentMatrix;
			}

			mNodes.push_back(myNode);
			myNode->ContainMeshesIndex.reserve(node->mNumMeshes);

			for (UINT i = 0; i < node->mNumMeshes; ++i)
			{
				unsigned int meshIndex = node->mMeshes[i];
				aiMesh* mesh = scene->mMeshes[meshIndex];

				myNode->ContainMeshesIndex.push_back(mSkinnedMeshes.size());
				mSkinnedMeshes.emplace_back(device, mesh);
			}

			for (UINT i = 0; i < node->mNumChildren; ++i)
			{
				nodeRecursive(node->mChildren[i], myNode);
			}
		};

		nodeRecursive(scene->mRootNode, nullptr);
		importer.FreeScene();

		// 본 계층구조의 node참조 인덱스 만들기
		map<string, size_t> nodeMap;

		for (const Node* node : mNodes)
		{
			nodeMap.insert({ node->Name, node->Index });
		}

		for (SkinnedMesh& mesh : mSkinnedMeshes)
		{
			for (Bone& bone : mesh.mBones)
			{
				auto finded = nodeMap.find(bone.Name);
				bone.NodeIndedx = finded->second;
			}
		}

		return true;
	}
}