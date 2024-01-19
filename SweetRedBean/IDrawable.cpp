#include "pch.h"

#include "IDrawable.h"
#include "IBindable.h"
#include "IndexBuffer.h"

void IDrawable::Draw(Graphics& gfx) const
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	for (auto& b : GetStaticBinds())
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void IDrawable::AddBind(std::unique_ptr<IBindable> bind)
{
	assert(typeid(*bind) != typeid(bind::IndexBuffer));
	binds.push_back(std::move(bind));
}
void IDrawable::AddIndexBuffer(std::unique_ptr<bind::IndexBuffer> ibuf)
{
	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
	pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}
