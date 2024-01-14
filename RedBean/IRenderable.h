#pragma once

namespace interfaceClass
{
	class IRenderable abstract
	{
	public:
		virtual void Render(ID3D11DeviceContext* context) = 0;
	};
};