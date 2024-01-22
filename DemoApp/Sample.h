#pragma once

#include "../Processor/Processor.h"
#include "../SweetRedBean/Camera.h"

#include "../SweetRedBean/Graphics.h"
#include "../SweetRedBean/IDrawable.h"
#include "../SweetRedBean/PointLight.h"
#include "../SweetRedBean/Model.h"
#include "../SweetRedBean/BlurOutlineRenderGraph.h"
#include "../SweetRedBean/ImguiManager.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace entryPoint
{
	using namespace common;

	class Sample final : public Processor
	{
	public:
		Sample(HINSTANCE hInstance, UINT width, UINT height, std::wstring name);
		virtual ~Sample() override;

		bool Init() override;
		void Update(float deltaTime) override;
		void Render() override;

		virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			{
				return true;
			}

			return Processor::MsgProc(hwnd, msg, wParam, lParam);
		}

	private:
		common::Camera mCamera;

		vector<unique_ptr<IDrawable>> mDrawables;

		unique_ptr<Graphics> mSweetGrapic;
		unique_ptr<Rgph::BlurOutlineRenderGraph> mRenderGraph;
		unique_ptr<::PointLight> mLight;
		unique_ptr<::Model> mModel;
		ImguiManager mImguiManager;
	};
}