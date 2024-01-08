#pragma once

#include "Processor.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

namespace entryPoint
{
	using namespace common;
	using namespace directXWrapper;

	class Sample final : public Processor
	{
	public:
		Sample(HINSTANCE hInstance, UINT width, UINT height, std::wstring name);

		bool Init() override;
		void Update(float deltaTime) override;
		void Render() override;

	private:
		VertexBuffer mVertexBuffer;
		IndexBuffer mIndexBuffer;
		ShaderProgram mShaderProgram;
	};
}