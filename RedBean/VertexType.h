#pragma once

// todo : 구조체 이름 수정

namespace common
{
	struct VertexData
	{
		Vector3 Position = { 0, 0, 0 };
	};

	struct VertexTextureData
	{
		Vector3 Position = { 0, 0, 0 };
		Vector2 UV = { 0, 0 };
	};

	struct VertexColorData
	{
		Vector3 Position = { 0, 0, 0 };
		SimpleMath::Color color = { 0, 0, 0, 0 };
	};

	struct VertexTextureNormalData
	{
		Vector3 Position = { 0, 0, 0 };
		Vector2 UV = { 0, 0 };
		Vector3 Normal = { 0, 0, 0 };
	};

	struct VertexTextureNormalTangentData
	{
		Vector3 Position = { 0, 0, 0 };
		Vector2 UV = { 0, 0 };
		Vector3 Normal = { 0, 0, 0 };
		Vector3 Tangent = { 0, 0, 0 };
	};

	struct VertexTextureNormalTangentBlendData
	{
		Vector3 Position = { 0, 0, 0 };
		Vector2 UV = { 0, 0 };
		Vector3 Normal = { 0, 0, 0 };
		Vector3 Tangent = { 0, 0, 0 };
		Vector4 BlendIndices = { 0, 0, 0, 0 };
		Vector4 BlendWeights = { 0, 0, 0, 0 };
	};

	using ModelVertexType = VertexTextureNormalTangentBlendData;

	class InputLayoutDesc
	{
	public:
		static const vector<D3D11_INPUT_ELEMENT_DESC> VertexData;
		static const vector<D3D11_INPUT_ELEMENT_DESC> VertexTextureData;
		static const vector<D3D11_INPUT_ELEMENT_DESC> VertexColorData;
		static const vector<D3D11_INPUT_ELEMENT_DESC> VertexTextureNormalData;
		static const vector<D3D11_INPUT_ELEMENT_DESC> VertexTextureNormalTangentData;
		static const vector<D3D11_INPUT_ELEMENT_DESC> VertexTextureNormalTangentBlendData;
	};
}
