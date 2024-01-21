#pragma once

#include "Graphics.h"
#include "Node.h"
#include "Mesh.h"

namespace Rgph
{
	class RenderGraph;
}

class Model
{
public:
	Model(Graphics& gfx, const std::string& pathString, float scale = 1.0f);
	~Model() = default;

	void Submit() const;
	void Accept(class ModelProbe& probe);
	void LinkTechniques(Rgph::RenderGraph& renderGraph);

	void SetRootTransform(Matrix tf);

private:
	// static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials, const std::filesystem::path& path, float scale);
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node, float scale);

private:
	std::unique_ptr<Node> mpRoot;
	// sharing meshes here perhaps dangerous?
	std::vector<std::unique_ptr<Mesh>> mMeshPtrs;
};