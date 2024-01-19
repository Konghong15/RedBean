#pragma once

#include "IBindable.h"

namespace bind
{
	class Topology : public IBindable
	{
	public:
		Topology(Graphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type)
			: mType(type)
		{
		}
		~Topology() = default;

		// static shared_ptr<Topology> Create(Graphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type);
		// static string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);

		virtual void Bind(Graphics& graphics)  override
		{
			GetContext(graphics)->IASetPrimitiveTopology(mType);
		}

		// string GetUID() const override;

	protected:
		D3D11_PRIMITIVE_TOPOLOGY mType;
	};
}

