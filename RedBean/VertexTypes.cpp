#include "pch.h"

#include "VertexTypes.h"
#include "ShaderPrograms.h"

namespace builtIn
{
	const vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc::VertexData =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	const vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc::PosTex =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV",   0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc::PosColor =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc::PosTexNormal =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV",   0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc::PosTexNormalTan =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV",   0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	const vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc::PosTexNormalTanSkinned =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV",   0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"INDICES",   0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3D11InputLayout* InputLayouts::PosTexNormalTan = nullptr;

	void InputLayouts::InitAll(ID3D11Device* device)
	{
		{
			ID3DBlob* skinnedProgramVSByteCode = ShaderPrograms::SkinnedModelProgram.GetVSByteCode().Get();
			HR(device->CreateInputLayout(
				&InputLayoutDesc::PosTexNormalTanSkinned[0],
				InputLayoutDesc::PosTexNormalTanSkinned.size(),
				skinnedProgramVSByteCode->GetBufferPointer(),
				skinnedProgramVSByteCode->GetBufferSize(),
				&PosTexNormalTan));
		}
	}

	void InputLayouts::DestroyAll()
	{
		ReleaseCOM(PosTexNormalTan);
	}
}