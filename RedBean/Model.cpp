#include "pch.h"

#include "Model.h"

#include "Node.h"
#include "Mesh.h"
#include "Animation.h"
#include "Material.h"

namespace resource
{
	bool Model::Init(NodeResource* nodeResource, MeshResource* meshResource, MaterialResource* materialResource)
	{
		// ���εǴ��� üũ
		if (meshResource->GetMeshes().size() != materialResource->GetMaterials().size())
		{
			return false;
		}

		mNodeResource = nodeResource;
		mMeshResource = meshResource;
		mMaterialResource = materialResource;

		return true;
	}

	bool SkinnedModel::Init(NodeResource* nodeResource, SkinnedMeshResource* skinnedMeshResource, MaterialResource* materialResource, AnimationResource* animationResource)
	{
		// ���εǴ��� üũ
		if (skinnedMeshResource->GetSkinnedMeshes().size() != materialResource->GetMaterials().size())
		{
			return false;
		}

		mNodeResource = nodeResource;
		mSkinnedMeshResource = skinnedMeshResource;
		mMaterialResource = materialResource;
		mAnimationResource = animationResource;

		// �� ���������� node���� �ε��� �����
		map<string, const Node*> nodeMap;

		for (const Node* node : nodeResource->GetNodes())
		{
			nodeMap.insert({ node->Name, node });
		}

		mBoneHierachy.clear();
		mBoneHierachy.reserve(skinnedMeshResource->GetSkinnedMeshes().size());

		for (const SkinnedMesh& mesh : skinnedMeshResource->GetSkinnedMeshes())
		{
			mBoneHierachy.push_back(vector<int>());
			vector<int>& curBoneHierachy = mBoneHierachy.back();
			curBoneHierachy.resize(mesh.mBones.size());

			map<string, unsigned int> boneIndexMap;
			for (const Bone& bone : mesh.mBones)
			{
				boneIndexMap.insert({ bone.Name, bone.Index });
			}

			for (const Bone& bone : mesh.mBones)
			{
				auto node = nodeMap.find(bone.Name);
				string& boneParentName = node->second->Parent->Name;
				auto parentBoneIndex = boneIndexMap.find(boneParentName);

				if (parentBoneIndex == boneIndexMap.end())
				{
					curBoneHierachy[bone.Index] = -1;
				}
				else
				{
					curBoneHierachy[bone.Index] = parentBoneIndex->second;
				}
			}
		}

		return true;
	}

	void SkinnedModel::Render(ID3D11DeviceContext* context, const string clipName, float timePos, const Matrix& worldMat)
	{

	}

	void SkinnedModelInstance::Render(ID3D11DeviceContext* context)
	{
		const auto& clips = ModelRef->GetAnimationResource()->GetAnimationClips();
		const auto& findAnimClip = clips.find(AnimationName);
		assert(findAnimClip != clips.end());

		const auto& skinnedMeshes = ModelRef->GetSkinnedMeshResource()->GetSkinnedMeshes();
		const auto& boneHierachy = ModelRef->GetBoneHierachy();

		for (size_t i = 0; i < skinnedMeshes.size(); ++i)
		{
			const SkinnedMesh& mesh = skinnedMeshes[i];
			const map<string, Keyframes>& animNodes = findAnimClip->second.mAnimationNodes;
			vector<Matrix> toParentMatrix(mesh.mBones.size());
			vector<Matrix> toRootMatrix(mesh.mBones.size());
			vector<Matrix> finalMatrix(mesh.mBones.size());

			// �ִϸ��̼� �������� ���� ����� �θ�ȯ����� �ȴ�.
			for (size_t j = 0; j < mesh.mBones.size(); ++j)
			{
				const Bone& bone = mesh.mBones[j];
				auto findAnimNode = animNodes.find(bone.Name);

				if (findAnimNode != animNodes.end())
				{
					toParentMatrix[bone.Index] = findAnimNode->second.Interpolate(TimePos);
				}
			}

			// ��Ʈ ��ȯ����� �����ش�.
			toRootMatrix[0] = toParentMatrix[0];
			for (size_t j = 1; j < mesh.mBones.size(); ++j)
			{
				const Bone& bone = mesh.mBones[j];
				size_t parentIndex = boneHierachy[i][bone.Index];
				toRootMatrix[bone.Index] = toParentMatrix[bone.Index] * toRootMatrix[parentIndex];
			}

			// ���� ��ȯ����� �� ������ �������� ��ȯ�� �ڿ� �Ͼ��.
			for (size_t j = 0; j < mesh.mBones.size(); ++j)
			{
				finalMatrix[j] = (mesh.mBones[j].OffsetMatrix * toRootMatrix[j]).Transpose();
			}

			builtIn::ShaderPrograms::SkinnedModelProgram.Bind(context);
			builtIn::ShaderPrograms::SkinnedModelProgram.UpdateSubresource(context);
			// updatesubresource boneMat
		}
	}
}