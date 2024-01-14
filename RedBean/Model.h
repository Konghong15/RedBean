#pragma once

#include "IRenderable.h"

namespace resource
{
	class NodeResource;
	class MeshResource;
	class MaterialResource;

	class Model
	{
	public:
		Model() = default;
		~Model() = default;

		bool Init(NodeResource* nodeResource, MeshResource* meshResource, MaterialResource* materialResource);

	private:
		NodeResource* mNodeResource;
		MeshResource* mMeshResource;
		MaterialResource* mMaterialResource;
	};

	struct ModelInstance : public interfaceClass::IRenderable
	{
		void Render(ID3D11DeviceContext* context) override;

		Model* ModelRef;
		Matrix WorldMatrix;
	};

	class SkinnedMeshResource;
	class AnimationResource;

	class SkinnedModel
	{
	public:
		SkinnedModel() = default;
		~SkinnedModel() = default;

		bool Init(NodeResource* nodeResource, SkinnedMeshResource* meshResource, MaterialResource* materialResource, AnimationResource* animationResource);
		void Render(ID3D11DeviceContext* context, const string clipName, float timePos, const Matrix& worldMat);

		NodeResource* GetNodeResource() const { return mNodeResource; }
		SkinnedMeshResource* GetSkinnedMeshResource() const { return mSkinnedMeshResource; }
		MaterialResource* GetMaterialResource() const { return mMaterialResource; }
		AnimationResource* GetAnimationResource() const { return mAnimationResource; }
		const vector<vector<int>>& GetBoneHierachy() const { return mBoneHierachy; }

	private:
		NodeResource* mNodeResource;
		SkinnedMeshResource* mSkinnedMeshResource;
		MaterialResource* mMaterialResource;
		AnimationResource* mAnimationResource;

		vector<vector<int>> mBoneHierachy; // Bone Parent index
	};

	struct SkinnedModelInstance : public interfaceClass::IRenderable
	{
		void Render(ID3D11DeviceContext* context) override;

		SkinnedModel* ModelRef;
		string AnimationName;
		float TimePos;
		Matrix WorldMatrix;
	};
}