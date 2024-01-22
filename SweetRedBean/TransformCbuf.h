#pragma once

#include "ConstantBuffer.h"
#include "IDrawable.h"
#include "IBindable.h"

/*
���� ���ۿ��� ����� MVP ��ȯ ����� �ٷ�� �������
*/

namespace Bind
{
	class TransformCbuf : public ICloningBindable
	{
	protected:
		struct Transforms
		{
			Matrix Model;
			Matrix ModelViewProj;
		};

	public:
		TransformCbuf(Graphics& graphics, UINT slot = 0u);

		void Bind(Graphics& graphics) override;
		void InitReference(const IDrawable& parent) override;

		std::unique_ptr<ICloningBindable> Clone() const override;

	protected:
		void updateBindImpl(Graphics& graphics, const Transforms& transform);
		Transforms getTransforms(Graphics& graphics);

	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
		const IDrawable* pParent = nullptr;
	};
}