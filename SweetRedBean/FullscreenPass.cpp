#include "pch.h"

#include "FullscreenPass.h"
#include "BindableCommon.h"

namespace Rgph
{
	FullscreenPass::FullscreenPass(const std::string name, Graphics& graphics)
		:
		BindingPass(std::move(name))
	{
		// setup fullscreen geometry
		Dvtx::VertexLayout lay;
		lay.Append(Dvtx::VertexLayout::Position2D);
		Dvtx::VertexBuffer bufFull{ lay };
		bufFull.EmplaceBack(Vector2{ -1,1 });
		bufFull.EmplaceBack(Vector2{ 1,1 });
		bufFull.EmplaceBack(Vector2{ -1,-1 });
		bufFull.EmplaceBack(Vector2{ 1,-1 });
		AddBind(Bind::VertexBuffer::Create(graphics, "$Full", std::move(bufFull)));
		std::vector<unsigned short> indices = { 0,1,2,1,3,2 };
		AddBind(Bind::IndexBuffer::Create(graphics, "$Full", std::move(indices)));
		// setup other common fullscreen bindables
		auto vs = Bind::VertexShader::Create(graphics, "Fullscreen_VS.cso");
		AddBind(Bind::InputLayout::Create(graphics, lay, vs->GetBytecode()));
		AddBind(std::move(vs));
		AddBind(Bind::Topology::Create(graphics));
		AddBind(Bind::Rasterizer::Create(graphics, false));
	}

	void FullscreenPass::Execute(Graphics& graphics) const
	{
		BindAll(graphics);
		graphics.DrawIndexed(6u);
	}
}
