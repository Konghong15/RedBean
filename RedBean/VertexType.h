#pragma once

// todo : 구조체 이름 수정

namespace common
{
	struct VertexData
	{
		Vector3 position = { 0, 0, 0 };
	};

	struct VertexTextureData
	{
		Vector3 position = { 0, 0, 0 };
		Vector2 uv = { 0, 0 };
	};

	struct VertexColorData
	{
		Vector3 position = { 0, 0, 0 };
		SimpleMath::Color color = { 0, 0, 0, 0 };
	};

	struct VertexTextureNormalData
	{
		Vector3 position = { 0, 0, 0 };
		Vector2 uv = { 0, 0 };
		Vector3 normal = { 0, 0, 0 };
	};

	struct VertexTextureNormalTangentData
	{
		Vector3 position = { 0, 0, 0 };
		Vector2 uv = { 0, 0 };
		Vector3 normal = { 0, 0, 0 };
		Vector3 tangent = { 0, 0, 0 };
	};

	struct VertexTextureNormalTangentBlendData
	{
		Vector3 position = { 0, 0, 0 };
		Vector2 uv = { 0, 0 };
		Vector3 normal = { 0, 0, 0 };
		Vector3 tangent = { 0, 0, 0 };
		Vector4 blendIndices = { 0, 0, 0, 0 };
		Vector4 blendWeights = { 0, 0, 0, 0 };
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
