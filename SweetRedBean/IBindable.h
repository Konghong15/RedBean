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
* ��� IBindable�� UID�� ���� ���ҽ��ν� �����ȴ�.
* IBindable�� friend Class�� �����Ͽ� Device�� Context ���� ������ ���������� ����д�.
*/