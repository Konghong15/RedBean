#include "pch.h"
#include "Node.h"

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

	bool NodeResource::Init(string filename)
	{
		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
		unsigned int importFlags = aiProcess_ConvertToLeftHanded;

		const aiScene* scene = importer.ReadFile(filename, importFlags);

		if (scene == nullptr)
		{
			return false;
		}

		mFilename = filename;
		mRoot = nullptr;
		mNodes.clear();
		mNodes.reserve(RESERVE_SIZE);

		function<void(aiNode*, Node*)> nodeRecursive = [&](aiNode* aiNode, Node* parent)
		{
			Node* node = new Node();
			node->Name = aiNode->mName.C_Str();
			node->Index = mNodes.size();
			node->ToParentMatrix = convertMatrix(aiNode->mTransformation).Transpose();

			if (parent != nullptr)
			{
				node->Parent = parent;
				node->ParentIndex = parent->Index;
				node->ToRootMatrix = node->ToParentMatrix * parent->ToRootMatrix;

				parent->Children.push_back(node);
			}
			else
			{
				node->Parent = nullptr;
				node->ParentIndex = Node::INVALID_INDEX;
				node->ToRootMatrix = node->ToParentMatrix;
			}

			mNodes.push_back(node);

			for (UINT i = 0; i < aiNode->mNumChildren; ++i)
			{
				nodeRecursive(aiNode->mChildren[i], node);
			}
		};

		nodeRecursive(scene->mRootNode, nullptr);
		assert(mNodes.size() > 0);
		mRoot = mNodes[0];

		importer.FreeScene();

		return true;
	}
}