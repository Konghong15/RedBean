#include "pch.h"

#include "RenderManager.h"

#include "IRenderable.h"

namespace renderSystem
{
	bool RenderManager::Init(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		md3dDevice = device;
		md3dContext = context;

		return true;
	}

	void RenderManager::RenderAll()
	{
		for (interfaceClass::IRenderable* renderable : mRenderables)
		{
			renderable->Render(md3dContext);
		}
	}
}