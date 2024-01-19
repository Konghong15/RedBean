#include "pch.h"

#include "Box.h"
#include "Cube.h"
#include "BindableCommon.h"
#include "Vertex.h"
#include "VertexLayout.h"

Box::Box(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	using namespace bind;

	if (!IsStaticInitialized())
	{
		using dynamic::VertexLayout;

		auto layout = VertexLayout{}.Append(VertexLayout::Position3D);
		//.Append(VertexLayout::Normal);

		dynamic::VertexBuffer vbuf(std::move(layout));

		struct Vertex
		{
			Vector3 pos;
		};
		const auto model = Cube::Make<Vertex>();

		for (Vertex vertex : model.vertices)
		{
			vbuf.EmplaceBack(vertex.pos);
		}

		// 내가 어떤 Vertex를 쓸 것인지 정의함
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		// 내가 어떤 쉐이더를 쓸 건지 정의함
		auto pvs = std::make_unique<VertexShader>(gfx, L"../SweetRedBean/ColorIndexVS.hlsl");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"../SweetRedBean/ColorIndexPS.hlsl"));

		// 내가 어떤 인덱스를 쓸 건지 정의함
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		struct PixelShaderConstants
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};
		const PixelShaderConstants cb2 =
		{
			{
				{ 1.0f,1.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,0.0f,1.0f },
				{ 0.0f,1.0f,1.0f },
				{ 0.0f,0.0f,0.0f },
			}
		};
		AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<bind::TransformConstantBuffer>(gfx, *this));

	// model deformation transform (per instance, not stored as bind)

	mt = Matrix::CreateScale(1, 1, bdist(rng));
}

void Box::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

Matrix Box::GetTransform() const noexcept
{
	namespace dx = DirectX;
	return mt *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
