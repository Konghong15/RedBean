#pragma once

namespace interfaceClass
{
	class IBindable abstract
	{
	public:
		virtual void Bind(ID3D11DeviceContext* context) = 0;
	};
}