#pragma once

#include "IRenderable.h"

namespace resource
{
	class MeshResource;
	class MaterialResource;

	class Model
	{
	public:
		Model() = default;
		~Model() = default;

		bool Init(MeshResource* meshResource, MaterialResource* materialResource);

		const MeshResource* GetMeshResource() const { return mMeshResource; }
		const MaterialResource* GetMaterialResource() const { return mMaterialResource; }

	private:
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

		bool Init(SkinnedMeshResource* meshResource, MaterialResource* materialResource, AnimationResource* animationResource);

		SkinnedMeshResource* GetSkinnedMeshResource() const { return mSkinnedMeshResource; }
		MaterialResource* GetMaterialResource() const { return mMaterialResource; }
		AnimationResource* GetAnimationResource() const { return mAnimationResource; }

	private:
		SkinnedMeshResource* mSkinnedMeshResource;
		MaterialResource* mMaterialResource;
		AnimationResource* mAnimationResource;
	};

	struct SkinnedModelInstance : public interfaceClass::IRenderable
	{
		void Render(ID3D11DeviceContext* context) override;

		SkinnedModel* ModelRef;
		string ClipName;
		float TimePos;
		Matrix WorldMatrix;
	};
}