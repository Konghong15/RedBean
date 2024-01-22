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
	Model(Graphics& grapics, const std::string& pathString, float scale = 1.0f);
	~Model() = default;

	void Submit() const;
	void Accept(class ModelProbe& probe);
	void LinkTechniques(Rgph::RenderGraph& renderGraph);

	void SetRootTransform(Matrix transform);

private:
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node, float scale);

private:
	std::unique_ptr<Node> mpRoot;
	std::vector<std::unique_ptr<Mesh>> mMeshPtrs;
};