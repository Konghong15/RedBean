#include "pch.h"

#include "TransformCbuf.h"
#include "Camera.h"

namespace Bind
{
	TransformCbuf::TransformCbuf(Graphics& graphics, UINT slot)
	{
		if (!pVcbuf)
		{
			pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(graphics, slot);
		}
	}

	void TransformCbuf::Bind(Graphics& graphics)
	{
		updateBindImpl(graphics, getTransforms(graphics));
	}

	void TransformCbuf::InitReference(const IDrawable& parent)
	{
		pParent = &parent;
	}

	std::unique_ptr<ICloningBindable> TransformCbuf::Clone() const
	{
		return std::make_unique<TransformCbuf>(*this);
	}

	void TransformCbuf::updateBindImpl(Graphics& graphics, const Transforms& transform)
	{
		assert(pParent != nullptr);
		pVcbuf->Update(graphics, transform);
		pVcbuf->Bind(graphics);
	}

	TransformCbuf::Transforms TransformCbuf::getTransforms(Graphics& graphics)
	{
		assert(pParent != nullptr);
		const auto modelView = pParent->GetTransform() * graphics.GetView();

		TransformCbuf::Transforms result =
		{
			pParent->GetTransform().Transpose(),
			(pParent->GetTransform() * graphics.GetView() * graphics.GetProjection()).Transpose()
		};

		return result;
	}

	std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;
}