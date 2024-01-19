#pragma once

#include "Graphics.h"
#include "IndexBuffer.h"

class IBindable;

class IDrawable
{
	template<class T>
	friend class DrawableBase;

public:
	IDrawable() = default;
	IDrawable(const IDrawable&) = delete;
	virtual ~IDrawable() = default;

	virtual void Update(float dt) = 0;
	void Draw(Graphics& gfx) const;

	virtual Matrix GetTransform() const = 0;

protected:
	void AddBind(std::unique_ptr<IBindable> bind);
	void AddIndexBuffer(std::unique_ptr<bind::IndexBuffer> ibuf);

private:
	virtual const std::vector<std::unique_ptr<IBindable>>& GetStaticBinds() const noexcept = 0;

private:
	const bind::IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<IBindable>> binds;
};