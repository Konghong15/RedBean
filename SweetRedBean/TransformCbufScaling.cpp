#include "pch.h"

#include "TransformCbufScaling.h"
#include "TechniqueProbe.h"

namespace Bind
{
	Bind::TransformCbufScaling::TransformCbufScaling(Graphics& graphics, float scale)
		: TransformCbuf(graphics)
		, mBuf(makeLayout())
	{
		mBuf["scale"] = scale;
	}

	void Bind::TransformCbufScaling::Accept(TechniqueProbe& probe)
	{
		probe.VisitBuffer(mBuf);
	}

	void Bind::TransformCbufScaling::Bind(Graphics& graphics)
	{
		const float scale = mBuf["scale"];

		const auto scaleMatrix = Matrix::CreateScale(scale, scale, scale);

		auto Transforms = getTransforms(graphics);
		Transforms.Model *= scaleMatrix;
		Transforms.ModelViewProj *= scaleMatrix;

		updateBindImpl(graphics, Transforms);
	}

	std::unique_ptr<ICloningBindable> Bind::TransformCbufScaling::Clone() const
	{
		return std::make_unique<TransformCbufScaling>(*this);
	}

	Dcb::RawLayout Bind::TransformCbufScaling::makeLayout()
	{
		Dcb::RawLayout layout;
		layout.Add<Dcb::Float>("scale");
		return layout;
	}
}