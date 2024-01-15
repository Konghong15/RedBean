#include "pch.h"

#include "RenderManager.h"

#include "IRenderable.h"
#include "ConstantBuffers.h"

namespace renderSystem
{
	bool RenderManager::Init(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		md3dDevice = device;
		md3dContext = context;

		mFrameInfo.bUseFog = false;
		mFrameInfo.LightCount = 1;
		mFrameInfo.DirLight->Ambient = { 0.1f, 0.1f, 0.1f,0.1f };
		mFrameInfo.DirLight->Diffuse = { 0.5f, 0.5f, 0.5f,0.1f };
		mFrameInfo.DirLight->Specular = { 0.5f, 0.5f, 0.5f, 8.f };
		mFrameInfo.DirLight->Direction = { 0.f, 0.f, 1.f, 0.f };

		return true;
	}

	void RenderManager::RenderAll()
	{
		mCamera.UpdateViewMatrix();

		mFrameInfo.EyePos = mCamera.GetPosition();
		builtIn::ConstantBuffers::FrameCB.UpdateSubResource(md3dContext, &mFrameInfo);

		builtIn::ConstantBuffers::TransformVP transformVP;
		transformVP.View = mCamera.GetView().Transpose();
		transformVP.Proj = mCamera.GetProj().Transpose();
		builtIn::ConstantBuffers::TransformVPCB.UpdateSubResource(md3dContext, &transformVP);

		for (interfaceClass::IRenderable* renderable : mRenderables)
		{
			renderable->Render(md3dContext);
		}
	}
}