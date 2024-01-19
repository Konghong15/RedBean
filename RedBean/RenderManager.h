#pragma once

#include "Model.h"
#include "ConstantBuffers.h"

namespace interfaceClass
{
	IRenderable;
}

namespace renderSystem
{
	using ViewProj = builtIn::ConstantBuffers::TransformVP;
	using FrameInfo = builtIn::ConstantBuffers::Frame;

	class RenderManager : public Singleton<RenderManager>
	{
		friend class Singleton<RenderManager>;

	public:
		bool Init(ID3D11Device* device, ID3D11DeviceContext* context);

		void RenderAll();

		// 파이프 라인을 바인딩 하는 기능을 열어놔서 맘대로 그릴 수 있게 하고,
		// 내가 정의한 모델 리소스를 받아서 렌더링할 수 있게 처리하면 되려나?
		// 근데 모델 리소스 받아서 렌더링할 수 있게 하려면 분기를를를를를를를를를 못하잖아.

		void Register(interfaceClass::IRenderable* renderable) { mRenderables.insert(renderable); }
		void Unregister(interfaceClass::IRenderable* renderable) { mRenderables.erase(renderable); }

		void SetViewProj(ViewProj viewProj) { mTransformVP = viewProj; }
		void SetFrameInfo(FrameInfo frameInfo) { mFrameInfo = frameInfo; }

		ViewProj& GetTransformVP() { return mTransformVP; }
		FrameInfo& GetFrameInfo() { return mFrameInfo; }

		const ViewProj& GetTransformVP() const { return mTransformVP; }
		const FrameInfo& GetFrameInfo() const { return mFrameInfo; }

	private:
		RenderManager() = default;
		~RenderManager() = default;
		RenderManager(const RenderManager&) = delete;
		RenderManager& operator=(const RenderManager&) = delete;

	private:
		ID3D11Device* md3dDevice;
		ID3D11DeviceContext* md3dContext;

		set<interfaceClass::IRenderable*> mRenderables;

		ViewProj mTransformVP;
		FrameInfo mFrameInfo;
	};
}