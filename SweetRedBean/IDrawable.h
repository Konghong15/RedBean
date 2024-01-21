#pragma once

#include "Graphics.h"
#include "Technique.h"

class TechniqueProbe;
class Material;
struct aiMesh;

namespace Rgph
{
	class RenderGraph;
}

namespace Bind
{
	class IndexBuffer;
	class VertexBuffer;
	class Topology;
	class InputLayout;
}

class IDrawable
{
public:
	IDrawable() = default;
	IDrawable(Graphics& graphics, const Material& mat, const aiMesh& mesh, float scale = 1.0f);
	virtual ~IDrawable() = default;
	IDrawable(const IDrawable&) = delete;

	void Submit() const;
	void Bind(Graphics& graphics) const;
	void Accept(TechniqueProbe& probe);

	void LinkTechniques(Rgph::RenderGraph&);
	void AddTechnique(Technique technique);

	virtual Matrix GetTransform() const = 0;
	UINT GetIndexCount() const;

protected:
	std::shared_ptr<Bind::IndexBuffer> pIndices;
	std::shared_ptr<Bind::VertexBuffer> pVertices;
	std::shared_ptr<Bind::Topology> pTopology;
	std::vector<Technique> techniques;
};