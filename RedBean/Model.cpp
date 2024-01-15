#include "pch.h"

#include "Model.h"

#include "Mesh.h"
#include "Animation.h"
#include "Material.h"
#include "Texture.h"

#include "IndexBuffer.h"

#include "ConstantBuffers.h"
#include "ShaderPrograms.h"

namespace resource
{
	bool Model::Init(MeshResource* meshResource, MaterialResource* materialResource)
	{
		// ���εǴ��� üũ
		if (meshResource->GetMeshes().size() != materialResource->GetMaterials().size())
		{
			return false;
		}

		mMeshResource = meshResource;
		mMaterialResource = materialResource;

		return true;
	}

	void ModelInstance::Render(ID3D11DeviceContext* context)
	{
		const MeshResource* meshResource = ModelRef->GetMeshResource();
		const vector<Node*>& nodes = meshResource->GetNodes();
		const vector<Mesh>& meshes = meshResource->GetMeshes();

		size_t currentMeshMatIndex = 0;

		for (Node* node : nodes)
		{
			for (size_t meshIndex : node->ContainMeshesIndex)
			{
				const Mesh& curMesh = meshes[meshIndex];

				curMesh.GetIB().Bind(context);
				curMesh.GetVB().Bind(context);

				// ��� ���� ����
				{
					builtIn::ConstantBuffers::TransformW transformW;
					transformW.World = (node->ToRootMatrix * WorldMatrix).Transpose();
					transformW.WorldInvTranspose = common::MathHelper::InverseTranspose(transformW.World).Transpose();

					auto transformWCB = builtIn::ConstantBuffers::TransformWCB.GetComPtr();
					context->UpdateSubresource(transformWCB.Get(), 0, 0, &transformW, 0, 0);
				}

				// �ؽ�ó ���ε�
				{
					const MaterialResource* materialResource = ModelRef->GetMaterialResource();
					const Material& material = materialResource->GetMaterials()[currentMeshMatIndex];
					const auto& textures = material.GetTextures();

					if (material.GetIsPBRTexture())
					{
						builtIn::ShaderPrograms::PBRBasicModelProgram.Bind(context);

						directXWrapper::Texture* textures[] =
						{
							material.GetTextureOrNull(eTexutreType::Diffuse),
							material.GetTextureOrNull(eTexutreType::Normal),
							material.GetTextureOrNull(eTexutreType::Specular),
							material.GetTextureOrNull(eTexutreType::Opacity),
							material.GetTextureOrNull(eTexutreType::Metalness),
							material.GetTextureOrNull(eTexutreType::Shininess),
						};

						builtIn::ConstantBuffers::PBRMaterial pbrMaterial;
						int* legacyMaterialPtr = reinterpret_cast<int*>(&pbrMaterial);
						for (auto* texture : textures)
						{
							*legacyMaterialPtr = texture != nullptr;
							++legacyMaterialPtr;
						}

						ID3D11ShaderResourceView* SRVs[] =
						{
							textures[0] == nullptr ? nullptr : textures[0]->GetComPtr().Get(),
							textures[1] == nullptr ? nullptr : textures[1]->GetComPtr().Get(),
							textures[2] == nullptr ? nullptr : textures[2]->GetComPtr().Get(),
							textures[3] == nullptr ? nullptr : textures[3]->GetComPtr().Get(),
							textures[4] == nullptr ? nullptr : textures[4]->GetComPtr().Get(),
							textures[5] == nullptr ? nullptr : textures[5]->GetComPtr().Get(),
						};

						context->PSSetShaderResources(0, ARRAYSIZE(SRVs), SRVs);

						auto materialCB = builtIn::ConstantBuffers::PBRMaterialCB.GetComPtr();
						context->UpdateSubresource(materialCB.Get(), 0, 0, &pbrMaterial, 0, 0);
					}
					else
					{
						builtIn::ShaderPrograms::BasicModelProgram.Bind(context);

						directXWrapper::Texture* textures[] =
						{
							material.GetTextureOrNull(eTexutreType::Diffuse),
							material.GetTextureOrNull(eTexutreType::Normal),
							material.GetTextureOrNull(eTexutreType::Specular),
							material.GetTextureOrNull(eTexutreType::Opacity),
						};

						builtIn::ConstantBuffers::LegacyMaterial legacyMaterial;
						int* legacyMaterialPtr = reinterpret_cast<int*>(&legacyMaterial);
						for (auto* texture : textures)
						{
							*legacyMaterialPtr = texture != nullptr;
							++legacyMaterialPtr;
						}

						ID3D11ShaderResourceView* SRVs[] =
						{
							textures[0] == nullptr ? nullptr : textures[0]->GetComPtr().Get(),
							textures[1] == nullptr ? nullptr : textures[1]->GetComPtr().Get(),
							textures[2] == nullptr ? nullptr : textures[2]->GetComPtr().Get(),
							textures[3] == nullptr ? nullptr : textures[3]->GetComPtr().Get(),
						};

						context->PSSetShaderResources(0, ARRAYSIZE(SRVs), SRVs);

						auto materialCB = builtIn::ConstantBuffers::LegacyMaterialCB.GetComPtr();
						context->UpdateSubresource(materialCB.Get(), 0, 0, &legacyMaterial, 0, 0);
					}
				}

				context->DrawIndexed(curMesh.GetIB().GetCount(), 0, 0);
				++currentMeshMatIndex;
			}
		}
	}

	bool SkinnedModel::Init(SkinnedMeshResource* skinnedMeshResource, MaterialResource* materialResource, AnimationResource* animationResource)
	{
		// ���εǴ��� üũ
		if (skinnedMeshResource->GetSkinnedMeshes().size() != materialResource->GetMaterials().size())
		{
			return false;
		}

		mSkinnedMeshResource = skinnedMeshResource;
		mMaterialResource = materialResource;
		mAnimationResource = animationResource;

		return true;
	}

	void SkinnedModelInstance::Render(ID3D11DeviceContext* context)
	{
		const SkinnedMeshResource* meshResource = ModelRef->GetSkinnedMeshResource();
		size_t currentMeshMatIndex = 0;

		for (const SkinnedMesh& skinnedMesh : meshResource->GetSkinnedMeshes())
		{
			const AnimationResource* animResource = ModelRef->GetAnimationResource();
			const AnimationClip& animClip = animResource->GetAnimationClip(ClipName);

			// ��� ���� ��� ����
			vector<Node*>& nodes = ModelRef->GetSkinnedMeshResource()->GetNodes();

			int findNodeCount = 0;
			int total = 0;
			for (const Bone& bone : skinnedMesh.mBones)
			{
				total++;
				const auto& nodeAnim = animClip.mAnimationNodes.find(bone.Name);

				if (nodeAnim != animClip.mAnimationNodes.end())
				{
					findNodeCount++;
					nodes[bone.NodeIndedx]->ToParentMatrix = nodeAnim->second.Interpolate(TimePos);
				}
			}

			// ��� ���� ��� ����
			nodes[0]->ToRootMatrix = nodes[0]->ToParentMatrix;

			for (size_t i = 1; i < nodes.size(); ++i)
			{
				nodes[i]->ToRootMatrix = nodes[i]->ToParentMatrix * nodes[i]->Parent->ToRootMatrix;
			}

			// �� �ȷ�Ʈ �����
			vector<Matrix> finalMatrix(128);
			auto finalMatrixIter = finalMatrix.begin();
			for (const Bone& bone : skinnedMesh.mBones)
			{
				Node* node = nodes[bone.NodeIndedx];
				*finalMatrixIter++ = (bone.OffsetMatrix * node->ToRootMatrix).Transpose();
			}

			// ������ ������Ʈ �� ���ε�

			// ���̴�, ���� ���ε�
			{
				skinnedMesh.mVB.Bind(context);
				skinnedMesh.mIB.Bind(context);
			}

			// ��� ���� ����
			{
				builtIn::ConstantBuffers::TransformW transformW;
				transformW.World = WorldMatrix.Transpose();
				transformW.WorldInvTranspose = common::MathHelper::InverseTranspose(WorldMatrix).Transpose();

				auto transformWCB = builtIn::ConstantBuffers::TransformWCB.GetComPtr();
				context->UpdateSubresource(transformWCB.Get(), 0, 0, &transformW, 0, 0);

				auto bonePaletteCB = builtIn::ConstantBuffers::BonePaletteCB.GetComPtr();
				context->UpdateSubresource(bonePaletteCB.Get(), 0, 0, &finalMatrix[0], 0, 0);
			}

			// �ؽ�ó ���ε�
			{
				const MaterialResource* materialResource = ModelRef->GetMaterialResource();
				const Material& material = materialResource->GetMaterials()[currentMeshMatIndex];
				const auto& textures = material.GetTextures();

				if (material.GetIsPBRTexture())
				{
					builtIn::ShaderPrograms::PBRSkinnedModelProgram.Bind(context);

					directXWrapper::Texture* textures[] =
					{
						material.GetTextureOrNull(eTexutreType::Diffuse),
						material.GetTextureOrNull(eTexutreType::Normal),
						material.GetTextureOrNull(eTexutreType::Specular),
						material.GetTextureOrNull(eTexutreType::Opacity),
						material.GetTextureOrNull(eTexutreType::Metalness),
						material.GetTextureOrNull(eTexutreType::Shininess),
					};

					builtIn::ConstantBuffers::PBRMaterial pbrMaterial;
					int* legacyMaterialPtr = reinterpret_cast<int*>(&pbrMaterial);
					for (auto* texture : textures)
					{
						*legacyMaterialPtr = texture != nullptr;
						++legacyMaterialPtr;
					}

					ID3D11ShaderResourceView* SRVs[] =
					{
						textures[0] == nullptr ? nullptr : textures[0]->GetComPtr().Get(),
						textures[1] == nullptr ? nullptr : textures[1]->GetComPtr().Get(),
						textures[2] == nullptr ? nullptr : textures[2]->GetComPtr().Get(),
						textures[3] == nullptr ? nullptr : textures[3]->GetComPtr().Get(),
						textures[4] == nullptr ? nullptr : textures[4]->GetComPtr().Get(),
						textures[5] == nullptr ? nullptr : textures[5]->GetComPtr().Get(),
					};

					context->PSSetShaderResources(0, ARRAYSIZE(SRVs), SRVs);

					auto materialCB = builtIn::ConstantBuffers::PBRMaterialCB.GetComPtr();
					context->UpdateSubresource(materialCB.Get(), 0, 0, &pbrMaterial, 0, 0);
				}
				else
				{
					builtIn::ShaderPrograms::SkinnedModelProgram.Bind(context);

					directXWrapper::Texture* textures[] =
					{
						material.GetTextureOrNull(eTexutreType::Diffuse),
						material.GetTextureOrNull(eTexutreType::Normal),
						material.GetTextureOrNull(eTexutreType::Specular),
						material.GetTextureOrNull(eTexutreType::Opacity),
					};

					builtIn::ConstantBuffers::LegacyMaterial legacyMaterial;
					int* legacyMaterialPtr = reinterpret_cast<int*>(&legacyMaterial);
					for (auto* texture : textures)
					{
						*legacyMaterialPtr = texture != nullptr;
						++legacyMaterialPtr;
					}

					ID3D11ShaderResourceView* SRVs[] =
					{
						textures[0] == nullptr ? nullptr : textures[0]->GetComPtr().Get(),
						textures[1] == nullptr ? nullptr : textures[1]->GetComPtr().Get(),
						textures[2] == nullptr ? nullptr : textures[2]->GetComPtr().Get(),
						textures[3] == nullptr ? nullptr : textures[3]->GetComPtr().Get(),
					};

					context->PSSetShaderResources(0, ARRAYSIZE(SRVs), SRVs);

					auto materialCB = builtIn::ConstantBuffers::LegacyMaterialCB.GetComPtr();
					context->UpdateSubresource(materialCB.Get(), 0, 0, &legacyMaterial, 0, 0);
				}
			}

			context->DrawIndexed(skinnedMesh.mIB.GetCount(), 0, 0);
			++currentMeshMatIndex;
		}
	}
}