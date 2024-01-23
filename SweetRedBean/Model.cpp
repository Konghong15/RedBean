#include "pch.h"

#include "Model.h"
#include "Node.h"
#include "Mesh.h"
#include "Material.h"

Model::Model(Graphics& grapics, const std::string& pathString, const float scale)
{
	Assimp::Importer imp;

	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
	unsigned int importFlags = aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords |
		aiProcess_CalcTangentSpace |
		aiProcess_LimitBoneWeights |
		aiProcess_ConvertToLeftHanded;

	const aiScene* pScene = importer.ReadFile(pathString, importFlags);

	if (pScene == nullptr)
	{
		assert(false);
	}

	// 재질 생성
	std::vector<Material> materials;
	materials.reserve(pScene->mNumMaterials);

	for (size_t i = 0; i < pScene->mNumMaterials; ++i)
	{
		materials.emplace_back(grapics, *pScene->mMaterials[i], pathString);
	}

	// 매쉬 생성
	for (size_t i = 0; i < pScene->mNumMeshes; ++i)
	{
		aiMesh& mesh = *pScene->mMeshes[i];
		unsigned int materialIndex = mesh.mMaterialIndex;
		Material& material = materials[mesh.mMaterialIndex];

		// 매핑된 매쉬로 랜더링
		mMeshPtrs.push_back(std::make_unique<Mesh>(grapics, material, mesh, scale));
	}

	// 노드 생성
	int nextId = 0;
	mpRoot = ParseNode(nextId, *pScene->mRootNode, scale);
}

void Model::Submit() const
{
	mpRoot->Submit(Matrix::Identity);
}

void Model::SetRootTransform(Matrix transform)
{
	mpRoot->SetAppliedTransform(transform);
}

void Model::Accept(ModelProbe& probe)
{
	mpRoot->Accept(probe);
}

void Model::LinkTechniques(Rgph::RenderGraph& renderGraph)
{
	for (auto& pMesh : mMeshPtrs)
	{
		pMesh->LinkTechniques(renderGraph);
	}
}

std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node, float scale)
{
	namespace dx = DirectX;

	Matrix transform = (reinterpret_cast<const Matrix*>(&node.mTransformation)->Transpose());

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);

	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const unsigned int MESH_INDEX = node.mMeshes[i];
		Mesh* meshPtr = mMeshPtrs[MESH_INDEX].get();

		curMeshPtrs.push_back(meshPtr);
	}

	auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);

	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->addChild(ParseNode(nextId, *node.mChildren[i], scale));
	}

	return pNode;
}
