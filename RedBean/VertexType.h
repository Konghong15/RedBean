#pragma once

// todo : 구조체 이름 수정

namespace builtIn
{
#pragma region Struct
	namespace vertex
	{
		struct Pos
		{
			Vector3 Position = { 0, 0, 0 };
		};

		struct PosTex
		{
			Vector3 Position = { 0, 0, 0 };
			Vector2 UV = { 0, 0 };
		};

		struct PosColor
		{
			Vector3 Position = { 0, 0, 0 };
			SimpleMath::Color color = { 0, 0, 0, 0 };
		};

		struct PosTexNormal
		{
			Vector3 Position = { 0, 0, 0 };
			Vector2 UV = { 0, 0 };
			Vector3 Normal = { 0, 0, 0 };
		};

		struct PosTexNormalTan
		{
			Vector3 Position = { 0, 0, 0 };
			Vector2 UV = { 0, 0 };
			Vector3 Normal = { 0, 0, 0 };
			Vector3 Tangent = { 0, 0, 0 };
		};

		struct PosTexNormalTanSkinned
		{
			Vector3 Position = { 0, 0, 0 };
			Vector2 UV = { 0, 0 };
			Vector3 Normal = { 0, 0, 0 };
			Vector3 Tangent = { 0, 0, 0 };
			Vector4 Indices = { 0, 0, 0, 0 };
			Vector4 Weights = { 0, 0, 0, 0 };
		};
	}

#pragma endregion

	class InputLayoutDesc
	{
	public:
		static const vector<D3D11_INPUT_ELEMENT_DESC> VertexData;
		static const vector<D3D11_INPUT_ELEMENT_DESC> PosTex;
		static const vector<D3D11_INPUT_ELEMENT_DESC> PosColor;
		static const vector<D3D11_INPUT_ELEMENT_DESC> PosTexNormal;
		static const vector<D3D11_INPUT_ELEMENT_DESC> PosTexNormalTan;
		static const vector<D3D11_INPUT_ELEMENT_DESC> PosTexNormalTanSkinned;
	};

	class InputLayouts
	{
	public:
		static void InitAll(ID3D11Device* device);
		static void DestroyAll();

		static ID3D11InputLayout* Pos;
		static ID3D11InputLayout* Basic32;
		static ID3D11InputLayout* PosNormalTexTan;
	};
}
