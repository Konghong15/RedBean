#include "pch.h"

#include "Sample.h"
#include "GeometryGenerator.h"

namespace entryPoint
{
	Sample::Sample(HINSTANCE hInstance, UINT width, UINT height, std::wstring name)
		: Processor(hInstance, width, height, name)
	{
	}

	bool Sample::Init()
	{
		if (!Processor::Init())
		{
			return false;
		}

		return true;
	}

	void Sample::Update(float deltaTime)
	{

	}

	void Sample::Render()
	{
		assert(md3dContext);
		assert(mSwapChain);

		float color[] = { 1.0f, 1.0f, 0.0f, 1.0f };

		mSwapChain->Present(0, 0);
	}
}