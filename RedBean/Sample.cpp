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

		common::GeometryGenerator::MeshData meshdata;
		common::GeometryGenerator::CreateBox(0.5, 0.5, 0.5, &meshdata);

		std::vector<VertexData> vertices;

		for (const auto& vertex : meshdata.Vertices)
		{
			vertices.push_back({ vertex.Position });
			vertices.back().position.z += 0.5;
		}

		mVertexBuffer.Init(md3dDevice, vertices);
		mIndexBuffer.Init(md3dDevice, meshdata.Indices);

		ShaderProgramDesc shaderProgramDesc;
		shaderProgramDesc.InputLayoutDescOrNull = &common::InputLayoutDesc::VertexData;
		shaderProgramDesc.VSDesc.szFileNameOrNull = L"TestVS.hlsl";
		shaderProgramDesc.PSDesc.szFileNameOrNull = L"TestPS.hlsl";
		mShaderProgram.Init(md3dDevice, shaderProgramDesc);

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

		md3dContext->ClearRenderTargetView(mRenderTargetView, color);
		md3dContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// shader program info, gpu가 어떻게 동작할지를 정의한다.
		mShaderProgram.Bind(md3dContext);

		// instance info, 어떤 오브젝트가 그려질지에 대한 정보를 정의한다.
		mVertexBuffer.Bind(md3dContext);
		mIndexBuffer.Bind(md3dContext);
		md3dContext->DrawIndexed(mIndexBuffer.GetCount(), 0, 0);

		mSwapChain->Present(0, 0);
	}
}