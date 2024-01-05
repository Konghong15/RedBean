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

		// ��� ���� ����� �� �����Ҵ�.
		PerFrame mPerFrame;
		PerObject mPerObject;
		directXWrapper::ConstantBuffer<PerFrame> mPerFrameCB;
		directXWrapper::ConstantBuffer<PerObject> mPerObjectCB;

		// ������ ���̴��� �� ���ε��� �Ŷ� �����Ҵ�.
		directXWrapper::ShaderProgram mShaderProgram;
		directXWrapper::IndexBuffer mIndexBuffer;
		directXWrapper::VertexBuffer<Vertex> mVertexBuffer;

		// ���ڿ� �ϳ� �����ϸ� �˾Ƽ� �����ǵ���
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
		// ���� x, y ��ǥ ������ ���� ��ĭ�� ������ ���Ѵ�.
		// �ʺ��� ������ x�� ���ϰ�, �� ũ��� �����ָ� �ε� �Ҽ��� ��ǥ�� ��������.
		float c = (x + 0.5f * GetWidth()) / mInfo.CellSpacing;
		float d = (z - 0.5f * GetDepth()) / -mInfo.CellSpacing;

		// �������� ���� ������ ���ؿ´�.
		int row = (int)floorf(d);
		int col = (int)floorf(c);

		// A B
		// C D
		float A = mHeightmap[row * mInfo.HeightmapWidth + col];
		float B = mHeightmap[row * mInfo.HeightmapWidth + col + 1];
		float C = mHeightmap[(row + 1) * mInfo.HeightmapWidth + col];
		float D = mHeightmap[(row + 1) * mInfo.HeightmapWidth + col + 1];

		// ���Ե� �ﰢ���� ���ϱ� ���� �� ���� ��ǥ�� ���Ѵ�.
		float s = c - (float)col;
		float t = d - (float)row;

		// �� ���� 1���� �۴ٴ� ���� ���� �ﰢ���� �ǹ��Ѵ�.
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