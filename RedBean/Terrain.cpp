#include "pch.h"

#include <fstream>
#include <sstream>

#include "Terrain.h"
#include "Camera.h"
#include "Effects.h"

namespace terrain
{
	Terrain::Terrain()
		: mNumPatchVertices(0)
		, mNumPatchQuadFaces(0)
		, mNumPatchVertRows(0)
		, mNumPatchVertCols(0)
	{
		mWorld = Matrix::Identity;

		mMat.Ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		mMat.Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		mMat.Specular = Vector4(0.0f, 0.0f, 0.0f, 64.0f);
		mMat.Reflect = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Terrain::Init(ID3D11Device* device, ID3D11DeviceContext* dc, const InitInfo& initInfo)
	{
		mInfo = initInfo;

		// 정점은 셀보다 하나 많아야 올바르게 생성된다.
		mNumPatchVertRows = ((mInfo.HeightmapHeight - 1) / CellsPerPatch) + 1;
		mNumPatchVertCols = ((mInfo.HeightmapWidth - 1) / CellsPerPatch) + 1;
		mNumPatchVertices = mNumPatchVertRows * mNumPatchVertCols;

		// Cell은 정점보다 하나 적게 생성된다. 위랑 똑같은 말이긴 함
		mNumPatchQuadFaces = (mNumPatchVertRows - 1) * (mNumPatchVertCols - 1);

		// height map 평활화 및 boundY 구하기
		loadHeightmap();
		smooth();
		calcAllPatchBoundsY();

		const vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		// 쉐이더 프로그램
		directXWrapper::ShaderProgramDesc shaderProgramDesc;
		shaderProgramDesc.InputLayoutDescOrNull = &inputLayoutDesc;
		shaderProgramDesc.VSDesc.FilePathOrNull = L"TerrainVS.hlsl";
		shaderProgramDesc.HSDesc.FilePathOrNull = L"TerrainHS.hlsl";
		shaderProgramDesc.DSDesc.FilePathOrNull = L"TerrainDS.hlsl";
		shaderProgramDesc.PSDesc.FilePathOrNull = L"TerrainPS.hlsl";
		mShaderProgram.Init(device, shaderProgramDesc);

		// 버퍼 생성
		mPerFrameCB.Init(device);
		mPerObjectCB.Init(device);
		buildQuadPatchVB(device);
		buildQuadPatchIB(device);

		// SRV 
		std::vector<std::wstring> layerFilenames;
		layerFilenames.push_back(mInfo.LayerMapFilename0);
		layerFilenames.push_back(mInfo.LayerMapFilename1);
		layerFilenames.push_back(mInfo.LayerMapFilename2);
		layerFilenames.push_back(mInfo.LayerMapFilename3);
		layerFilenames.push_back(mInfo.LayerMapFilename4);
		mTileArraySRV.Init(device, layerFilenames);
		mBlendMapSRV.Init(device, mInfo.BlendMapFilename);
		buildHeightmapSRV(device);
	}

	void Terrain::Draw(ID3D11DeviceContext* dc, const common::Camera& cam, DirectionLight lights[3])
	{

	}

	void Terrain::loadHeightmap()
	{
		std::vector<unsigned char> in(mInfo.HeightmapWidth * mInfo.HeightmapHeight);

		std::ifstream inFile;
		inFile.open(mInfo.HeightMapFilename.c_str(), std::ios_base::binary);

		if (inFile)
		{
			inFile.read((char*)&in[0], (std::streamsize)in.size());

			inFile.close();
		}

		mHeightmap.resize(mInfo.HeightmapHeight * mInfo.HeightmapWidth, 0);
		for (UINT i = 0; i < mInfo.HeightmapHeight * mInfo.HeightmapWidth; ++i)
		{
			// 데이터를 정규화 한 후 scale로 조정한다.
			mHeightmap[i] = (in[i] / 255.0f) * mInfo.HeightScale;
		}
	}

	void Terrain::smooth()
	{
		std::vector<float> dest(mHeightmap.size());

		for (UINT i = 0; i < mInfo.HeightmapHeight; ++i)
		{
			for (UINT j = 0; j < mInfo.HeightmapWidth; ++j)
			{
				// average는 (현재 픽셀 + sum(인접한 픽셀 값)) / 개수
				dest[i * mInfo.HeightmapWidth + j] = average(i, j);
			}
		}

		mHeightmap = dest;
	}

	bool Terrain::inBounds(int i, int j)
	{
		return
			i >= 0 && i < (int)mInfo.HeightmapHeight &&
			j >= 0 && j < (int)mInfo.HeightmapWidth;
	}

	float Terrain::average(int i, int j)
	{
		float avg = 0.0f;
		float num = 0.0f;

		for (int m = i - 1; m <= i + 1; ++m)
		{
			for (int n = j - 1; n <= j + 1; ++n)
			{
				if (inBounds(m, n))
				{
					avg += mHeightmap[m * mInfo.HeightmapWidth + n];
					num += 1.0f;
				}
			}
		}

		return avg / num;
	}

	void Terrain::calcAllPatchBoundsY()
	{
		mPatchBoundsY.resize(mNumPatchQuadFaces);

		// 모든 패치(셀 단위)를 순회하여 Bound Y를 구한다.
		for (UINT i = 0; i < mNumPatchVertRows - 1; ++i)
		{
			for (UINT j = 0; j < mNumPatchVertCols - 1; ++j)
			{
				calcPatchBoundsY(i, j);
			}
		}
	}

	void Terrain::calcPatchBoundsY(UINT i, UINT j)
	{
		// 셀 안에 들어간 패치만큼 순회하여 min, map를 찾는다.
		UINT x0 = j * CellsPerPatch;
		UINT x1 = (j + 1) * CellsPerPatch;

		UINT y0 = i * CellsPerPatch;
		UINT y1 = (i + 1) * CellsPerPatch;

		float minY = +MathHelper::Infinity;
		float maxY = -MathHelper::Infinity;

		for (UINT y = y0; y <= y1; ++y)
		{
			for (UINT x = x0; x <= x1; ++x)
			{
				UINT k = y * mInfo.HeightmapWidth + x;
				minY = MathHelper::Min(minY, mHeightmap[k]);
				maxY = MathHelper::Max(maxY, mHeightmap[k]);
			}
		}

		UINT patchID = i * (mNumPatchVertCols - 1) + j;
		mPatchBoundsY[patchID] = Vector2(minY, maxY);
	}

	void Terrain::buildQuadPatchVB(ID3D11Device* device)
	{
		// 패치 제어점 만큼 정점 생성
		std::vector<Vertex> patchVertices(mNumPatchVertRows * mNumPatchVertCols);

		float halfWidth = 0.5f * GetWidth();
		float halfDepth = 0.5f * GetDepth();

		// 셀의 너비는 (전체 셀 너비 / 샐 개수)
		float patchWidth = GetWidth() / (mNumPatchVertCols - 1);
		float patchDepth = GetDepth() / (mNumPatchVertRows - 1);
		float du = 1.0f / (mNumPatchVertCols - 1);
		float dv = 1.0f / (mNumPatchVertRows - 1);

		for (UINT i = 0; i < mNumPatchVertRows; ++i)
		{
			float z = halfDepth - i * patchDepth; // 화면 깊은쪽 부터

			for (UINT j = 0; j < mNumPatchVertCols; ++j)
			{
				float x = -halfWidth + j * patchWidth; // 왼쪽 부터

				patchVertices[i * mNumPatchVertCols + j].Pos = Vector3(x, 0.0f, z);

				patchVertices[i * mNumPatchVertCols + j].Tex.x = j * du;
				patchVertices[i * mNumPatchVertCols + j].Tex.y = i * dv;
			}
		}

		for (UINT i = 0; i < mNumPatchVertRows - 1; ++i)
		{
			for (UINT j = 0; j < mNumPatchVertCols - 1; ++j)
			{
				// 패치를 감싸는 높이의 최소/최대를 미리 계산한 것을 적용해준다.
				UINT patchID = i * (mNumPatchVertCols - 1) + j;
				patchVertices[i * mNumPatchVertCols + j].BoundsY = mPatchBoundsY[patchID];
			}
		}

		mVertexBuffer.Init(device, patchVertices);
	}

	void Terrain::buildQuadPatchIB(ID3D11Device* device)
	{
		// 제어점 패치 형태라서 Face당 4개의 색인을 갖는다. 
		std::vector<UINT> indices(mNumPatchQuadFaces * 4);

		int k = 0;
		for (UINT i = 0; i < mNumPatchVertRows - 1; ++i)
		{
			for (UINT j = 0; j < mNumPatchVertCols - 1; ++j)
			{
				// 윗 정점 두개
				indices[k] = i * mNumPatchVertCols + j;
				indices[k + 1] = i * mNumPatchVertCols + j + 1;

				// 아랫 정점 두개
				indices[k + 2] = (i + 1) * mNumPatchVertCols + j;
				indices[k + 3] = (i + 1) * mNumPatchVertCols + j + 1;

				k += 4;
			}
		}

		mIndexBuffer.Init(device, indices);
	}

	void Terrain::buildHeightmapSRV(ID3D11Device* device)
	{
		// 예외적인 것은 클래스로 만들지 않음

		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = mInfo.HeightmapWidth;
		texDesc.Height = mInfo.HeightmapHeight;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		// 16비트 float 만들기 로직

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = &mHeightmap[0];
		data.SysMemPitch = mInfo.HeightmapWidth * sizeof(float);
		data.SysMemSlicePitch = 0;

		ID3D11Texture2D* hmapTex = 0;
		HR(device->CreateTexture2D(&texDesc, &data, &hmapTex));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1; // 최대 밉맵 수, -1은 모든 밉맵 레벨 표시
		HR(device->CreateShaderResourceView(hmapTex, &srvDesc, mHeightMapSRV.GetAddressOf()));

		ReleaseCOM(hmapTex);
	}
}