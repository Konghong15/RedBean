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
		// 매핑되는지 체크
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
		// 매핑되는지 체크
		if (skinnedMeshResource->GetSkinnedMeshes().size() != materialResource->GetMaterials().size())
		{
			return false;
		}

		mNodeResource = nodeResource;
		mSkinnedMeshResource = skinnedMeshResource;
		mMaterialResource = materialResource;
		mAnimationResource = animationResource;

		// 본 계층구조의 node참조 인덱스 만들기
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

			// 애니메이션 보간으로 만든 행렬은 부모변환행렬이 된다.
			for (size_t j = 0; j < mesh.mBones.size(); ++j)
			{
				const Bone& bone = mesh.mBones[j];
				auto findAnimNode = animNodes.find(bone.Name);

				if (findAnimNode != animNodes.end())
				{
					toParentMatrix[bone.Index] = findAnimNode->second.Interpolate(TimePos);
				}
			}

			// 루트 변환행렬을 구해준다.
			toRootMatrix[0] = toParentMatrix[0];
			for (size_t j = 1; j < mesh.mBones.size(); ++j)
			{
				const Bone& bone = mesh.mBones[j];
				size_t parentIndex = boneHierachy[i][bone.Index];
				toRootMatrix[bone.Index] = toParentMatrix[bone.Index] * toRootMatrix[parentIndex];
			}

			// 최종 변환행렬은 본 오프셋 공간으로 변환된 뒤에 일어난다.
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