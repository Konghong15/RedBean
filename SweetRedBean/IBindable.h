#pragma once

class Graphics;

class IBindable
{
public:
	virtual ~IBindable() = default;

	virtual void Bind(Graphics& graphics) = 0;
	virtual void InitReference(const class IDrawable& drawable) {};

	virtual inline string GetUID() const;

protected:
	static ID3D11Device* GetDevice(Graphics& graphics);
	static ID3D11DeviceContext* GetContext(Graphics& graphics);
};

string IBindable::GetUID() const
{
	assert(false);
	return "";
}

/*
* 모든 IBindable은 UID를 통해 리소스로써 관리된다.
* IBindable만 friend Class로 지정하여 Device와 Context 접근 권한을 제한적으로 열어둔다.
*/