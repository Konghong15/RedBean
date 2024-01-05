#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "ShaderProgram.h"
#include "Texture.h"

namespace terrain
{
	using namespace common;

	class Terrain
	{
	public:
#pragma region Struct
		struct Vertex
		{
			Vector3 Pos;
			Vector2 Tex;
			Vector2 BoundsY;
		};

		struct PerObject
		{
			Matrix ViewProj;
			Material Material;
		};

		struct PerFrame
		{
			common::DirectionLight DirLights[3];
			Vector3 EyePosW;
			float  FogStart;
			Vector4 FogColor;
			float  FogRange;
			float MinDist;
			float MaxDist;
			float MinTess;
			float MaxTess;
			float TexelCellSpaceU;
			float TexelCellSpaceV;
			float WorldCellSpace;
			Vector4 WorldFrustumPlanes[6];
			Vector2 TexScale; // = 50.0f;
			float unuserd[2];
		};

		struct InitInfo
		{
			std::wstring HeightMapFilename;
			std::wstring LayerMapFilename0;
			std::wstring LayerMapFilename1;
			std::wstring LayerMapFilename2;
			std::wstring LayerMapFilename3;
			std::wstring LayerMapFilename4;
			std::wstring BlendMapFilename;
			float HeightScale;
			UINT HeightmapWidth;
			UINT HeightmapHeight;
			float CellSpacing;
		};
#pragma endregion

	public:
		Terrain();
		~Terrain() = default;

		void Init(ID3D11Device* device, ID3D11DeviceContext* dc, const InitInfo& initInfo);
		void Draw(ID3D11DeviceContext* dc, const Camera& cam, DirectionLight lights[3]);

		inline void SetWorld(Matrix M);

		inline float GetWidth() const;
		inline float GetDepth() const;
		inline float GetHeight(float x, float z) const;
		inline Matrix GetWorld()const;

	private:
		void buildTerrain(ID3D11Device* device);

		void loadHeightmap();
		void smooth();
		bool inBounds(int i, int j);
		float average(int i, int j);
		void calcAllPatchBoundsY();
		void calcPatchBoundsY(UINT i, UINT j);
		void buildQuadPatchVB(ID3D11Device* device);
		void buildQuadPatchIB(ID3D11Device* device);
		void buildHeightmapSRV(ID3D11Device* device);

	private:
		static const int CellsPerPatch = 64;

		// 상수 버퍼 만드는 게 귀찮았다.
		PerFrame mPerFrame;
		PerObject mPerObject;
		directXWrapper::ConstantBuffer<PerFrame> mPerFrameCB;
		directXWrapper::ConstantBuffer<PerObject> mPerObjectCB;

		// 생성한 셰이더를 다 바인딩할 거라 귀찮았다.
		directXWrapper::ShaderProgram mShaderProgram;
		directXWrapper::IndexBuffer mIndexBuffer;
		directXWrapper::VertexBuffer<Vertex> mVertexBuffer;

		// 문자열 하나 전달하면 알아서 생성되도록
		directXWrapper::Texture mTileArraySRV;
		directXWrapper::Texture mBlendMapSRV;
		ComPtr<ID3D11ShaderResourceView> mHeightMapSRV;

		InitInfo mInfo;

		UINT mNumPatchVertices;
		UINT mNumPatchQuadFaces;

		UINT mNumPatchVertRows;
		UINT mNumPatchVertCols;

		Matrix mWorld;

		Material mMat;

		std::vector<Vector2> mPatchBoundsY;
		std::vector<float> mHeightmap;
	};

	void Terrain::SetWorld(Matrix M)
	{
		mWorld = M;
	}

	float Terrain::GetWidth()const
	{
		return (mInfo.HeightmapWidth - 1) * mInfo.CellSpacing;
	}
	float Terrain::GetDepth()const
	{
		return (mInfo.HeightmapHeight - 1) * mInfo.CellSpacing;
	}
	float Terrain::GetHeight(float x, float z)const
	{
		// 월드 x, y 좌표 정보를 통해 낱칸의 색인을 구한다.
		// 너비의 절반을 x에 곱하고, 셀 크기로 나눠주면 부동 소수점 좌표가 구해진다.
		float c = (x + 0.5f * GetWidth()) / mInfo.CellSpacing;
		float d = (z - 0.5f * GetDepth()) / -mInfo.CellSpacing;

		// 버림으로 낮은 색인을 구해온다.
		int row = (int)floorf(d);
		int col = (int)floorf(c);

		// A B
		// C D
		float A = mHeightmap[row * mInfo.HeightmapWidth + col];
		float B = mHeightmap[row * mInfo.HeightmapWidth + col + 1];
		float C = mHeightmap[(row + 1) * mInfo.HeightmapWidth + col];
		float D = mHeightmap[(row + 1) * mInfo.HeightmapWidth + col + 1];

		// 포함된 삼각형을 구하기 위해 셀 내의 좌표를 구한다.
		float s = c - (float)col;
		float t = d - (float)row;

		// 이 합이 1보다 작다는 것은 위쪽 삼각형을 의미한다.
		if (s + t <= 1.0f)
		{
			// A -> B
			// C
			float uy = B - A;
			float vy = C - A;

			return A + s * uy + t * vy;
		}
		else
		{
			//      B
			// C <- D
			float uy = C - D;
			float vy = B - D;

			return D + (1.0f - s) * uy + (1.0f - t) * vy;
		}
	}
	Matrix Terrain::GetWorld()const
	{
		return mWorld;
	}
}