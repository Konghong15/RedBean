#include "pch.h"

#include <assimp/scene.h>

#include "IDrawable.h"
#include "BindableCommon.h"
#include "BindableResourceManager.h"
#include "Material.h"

using namespace Bind;

IDrawable::IDrawable(Graphics& graphics, const Material& mat, const aiMesh& mesh, float scale)
{
	// 매테리얼 정보로부터 정점과 인덱스를 만든다.
	pVertices = mat.MakeVertexBindable(graphics, mesh, scale);
	pIndices = mat.MakeIndexBindable(graphics, mesh);
	pTopology = Bind::Topology::Create(graphics);

	for (auto& t : mat.GetTechniques())
	{
		AddTechnique(std::move(t));
	}
}

void IDrawable::Submit() const
{
	for (const auto& technique : techniques)
	{
		technique.Submit(*this);
	}
}

void IDrawable::Bind(Graphics& graphics) const
{
	pTopology->Bind(graphics);
	pIndices->Bind(graphics);
	pVertices->Bind(graphics);
}

void IDrawable::Accept(TechniqueProbe& probe)
{
	for (auto& technique : techniques)
	{
		technique.Accept(probe);
	}
}

void IDrawable::LinkTechniques(Rgph::RenderGraph& rg)
{
	for (auto& tech : techniques)
	{
		tech.Link(rg);
	}
}

void IDrawable::AddTechnique(Technique technique) 
{
	technique.InitReferences(*this);
	techniques.push_back(std::move(technique));
}

UINT IDrawable::GetIndexCount() const
{
	return pIndices->GetCount();
}
