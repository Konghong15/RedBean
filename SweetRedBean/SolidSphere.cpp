#include "pch.h"

#include "SolidSphere.h"
#include "BindableCommon.h"
#include "Vertex.h"
#include "Sphere.h"
#include "Stencil.h"
#include "TransformCbuf.h"

SolidSphere::SolidSphere(Graphics& grapics, float radius)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = Sphere::Make();
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));
	const auto geometryTag = "$sphere." + std::to_string(radius);
	pVertices = VertexBuffer::Create(grapics, geometryTag, model.vertices);
	pIndices = IndexBuffer::Create(grapics, geometryTag, model.indices);
	pTopology = Topology::Create(grapics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	{
		Technique solid;
		Step only("NonPBR");

		auto pvs = VertexShader::Create(grapics, "../SweetRedBean/Solid_VS.hlsl");
		auto pvsbc = pvs->GetBytecode();
		only.AddBindable(std::move(pvs));

		only.AddBindable(PixelShader::Create(grapics, "../SweetRedBean/Solid_PS.hlsl"));

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
			float padding;
		} colorConst;
		only.AddBindable(PixelConstantBuffer<PSColorConstant>::Create(grapics, colorConst, 1u));

		only.AddBindable(InputLayout::Create(grapics, model.vertices.GetLayout(), pvsbc));

		only.AddBindable(std::make_shared<TransformCbuf>(grapics));

		only.AddBindable(Rasterizer::Create(grapics, false));

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
