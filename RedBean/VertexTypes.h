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
			enum { INVALID_INDEX = -1 };

			Vector3 Position = { 0, 0, 0 };
			Vector2 UV = { 0, 0 };
			Vector3 Normal = { 0, 0, 0 };
			Vector3 Tangent = { 0, 0, 0 };
			int Indices[4] = { INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX };
			float Weights[4] = { 0.f, };
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

		static ID3D11InputLayout* PosTexNormalTan;
	};
}
