#pragma once

#include "Graphics.h"
#include "Technique.h"
#include "Vertex.h"

struct aiMaterial;
struct aiMesh;

namespace Bind
{
	class VertexBuffer;
	class IndexBuffer;
}

class Material
{
public:
	Material(Graphics& graphics, const aiMaterial& material, const std::filesystem::path& path);
	~Material() = default;

	std::shared_ptr<Bind::VertexBuffer> MakeVertexBindable(Graphics& graphics, const aiMesh& mesh, float scale = 1.0f) const;
	std::shared_ptr<Bind::IndexBuffer> MakeIndexBindable(Graphics& graphics, const aiMesh& mesh) const;

	Dvtx::VertexBuffer ExtractVertices(const aiMesh& mesh) const;
	std::vector<unsigned short> ExtractIndices(const aiMesh& mesh) const;

	std::vector<Technique> GetTechniques() const;

private:
	std::string makeMeshTag(const aiMesh& mesh) const;

private:
	Dvtx::VertexLayout mVertexLayout;
	std::vector<Technique> mTechniques;
	std::string mModelPath;
	std::string mName;
};