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

		// ������ ������ ���ε� �ϴ� ����� ������� ����� �׸� �� �ְ� �ϰ�,
		// ���� ������ �� ���ҽ��� �޾Ƽ� �������� �� �ְ� ó���ϸ� �Ƿ���?
		// �ٵ� �� ���ҽ� �޾Ƽ� �������� �� �ְ� �Ϸ��� �б⸦���������������� �����ݾ�.

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