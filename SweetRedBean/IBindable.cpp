#include "pch.h"

#include "IBindable.h"
#include "Graphics.h"

ID3D11Device* IBindable::GetDevice(Graphics& graphics)
{
	return graphics.mpDevice.Get();
}

ID3D11DeviceContext* IBindable::GetContext(Graphics& graphics)
{
	return graphics.mpContext.Get();
}