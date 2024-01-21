#include "pch.h"

#include "SolidSphere.h"
#include "BindableCommon.h"
#include "Vertex.h"
#include "Sphere.h"
#include "Stencil.h"
#include "TransformCbuf.h"

SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = Sphere::Make();
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));
	const auto geometryTag = "$sphere." + std::to_string(radius);
	pVertices = VertexBuffer::Create(gfx, geometryTag, model.vertices);
	pIndices = IndexBuffer::Create(gfx, geometryTag, model.indices);
	pTopology = Topology::Create(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	{
		Technique solid;
		Step only("lambertian");

		auto pvs = VertexShader::Create(gfx, "Solid_VS.cso");
		auto pvsbc = pvs->GetBytecode();
		only.AddBindable(std::move(pvs));

		only.AddBindable(PixelShader::Create(gfx, "Solid_PS.cso"));

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
			float padding;
		} colorConst;
		only.AddBindable(PixelConstantBuffer<PSColorConstant>::Create(gfx, colorConst, 1u));

		only.AddBindable(InputLayout::Create(gfx, model.vertices.GetLayout(), pvsbc));

		only.AddBindable(std::make_shared<TransformCbuf>(gfx));

		only.AddBindable(Rasterizer::Create(gfx, false));

		solid.AddStep(std::move(only));
		AddTechnique(std::move(solid));
	}
}

void SolidSphere::SetPos(Vector3 pos)
{
	this->pos = pos;
}

Matrix SolidSphere::GetTransform() const
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
