#pragma once

#include "Model.h"

namespace interfaceClass
{
	IRenderable;
}

namespace renderSystem
{
	class RenderManager : public Singleton<RenderManager>
	{
		friend class Singleton<RenderManager>;

	public:
		bool Init(ID3D11Device* device, ID3D11DeviceContext* context);
		void RenderAll();

		void Register(interfaceClass::IRenderable* renderable) { mRenderables.insert(renderable); }
		void Unregister(interfaceClass::IRenderable* renderable) { mRenderables.erase(renderable); }

		common::Camera& GetCamera() { return mCamera; }
		const common::Camera& GetCamera() const { return mCamera; }

	private:
		RenderManager() = default;
		~RenderManager() = default;
		RenderManager(const RenderManager&) = delete;
		RenderManager& operator=(const RenderManager&) = delete;

	private:
		ID3D11Device* md3dDevice;
		ID3D11DeviceContext* md3dContext;

		set<interfaceClass::IRenderable*> mRenderables;
		common::Camera mCamera;
	};
}