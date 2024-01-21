#pragma once

#include "TransformCbuf.h"
#include "DynamicConstant.h"

/*
* 테크닉 프로브로 Scale을 제어할 수 있는 Transform CBuffer
*/

namespace Bind
{
	class TransformCbufScaling : public TransformCbuf
	{
	public:
		TransformCbufScaling(Graphics& graphics, float scale);
		virtual ~TransformCbufScaling() = default;

		void Accept(TechniqueProbe& probe) override;
		void Bind(Graphics& graphics)   override;

		std::unique_ptr<ICloningBindable> Clone() const override;

	private:
		static Dcb::RawLayout makeLayout();

	private:
		Dcb::Buffer mBuf;
	};
}