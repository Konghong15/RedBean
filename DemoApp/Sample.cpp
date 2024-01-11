#include "pch.h"

#include "Sample.h"

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
	}
}