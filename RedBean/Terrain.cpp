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

		// ������ ������ �ϳ� ���ƾ� �ùٸ��� �����ȴ�.
		mNumPatchVertRows = ((mInfo.HeightmapHeight - 1) / CellsPerPatch) + 1;
		mNumPatchVertCols = ((mInfo.HeightmapWidth - 1) / CellsPerPatch) + 1;
		mNumPatchVertices = mNumPatchVertRows * mNumPatchVertCols;

		// Cell�� �������� �ϳ� ���� �����ȴ�. ���� �Ȱ��� ���̱� ��
		mNumPatchQuadFaces = (mNumPatchVertRows - 1) * (mNumPatchVertCols - 1);

		// height map ��Ȱȭ �� boundY ���ϱ�
		loadHeightmap();
		smooth();
		calcAllPatchBoundsY();

		const vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		// ���̴� ���α׷�
		directXWrapper::ShaderProgramDesc shaderProgramDesc;
		shaderProgramDesc.InputLayoutDescOrNull = &inputLayoutDesc;
		shaderProgramDesc.VSDesc.FilePathOrNull = L"TerrainVS.hlsl";
		shaderProgramDesc.HSDesc.FilePathOrNull = L"TerrainHS.hlsl";
		shaderProgramDesc.DSDesc.FilePathOrNull = L"TerrainDS.hlsl";
		shaderProgramDesc.PSDesc.FilePathOrNull = L"TerrainPS.hlsl";
		mShaderProgram.Init(device, shaderProgramDesc);

		// ���� ����
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
			// �����͸� ����ȭ �� �� scale�� �����Ѵ�.
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
				// average�� (���� �ȼ� + sum(������ �ȼ� ��)) / ����
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

		// ��� ��ġ(�� ����)�� ��ȸ�Ͽ� Bound Y�� ���Ѵ�.
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
		// �� �ȿ� �� ��ġ��ŭ ��ȸ�Ͽ� min, map�� ã�´�.
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
		// ��ġ ������ ��ŭ ���� ����
		std::vector<Vertex> patchVertices(mNumPatchVertRows * mNumPatchVertCols);

		float halfWidth = 0.5f * GetWidth();
		float halfDepth = 0.5f * GetDepth();

		// ���� �ʺ�� (��ü �� �ʺ� / �� ����)
		float patchWidth = GetWidth() / (mNumPatchVertCols - 1);
		float patchDepth = GetDepth() / (mNumPatchVertRows - 1);
		float du = 1.0f / (mNumPatchVertCols - 1);
		float dv = 1.0f / (mNumPatchVertRows - 1);

		for (UINT i = 0; i < mNumPatchVertRows; ++i)
		{
			float z = halfDepth - i * patchDepth; // ȭ�� ������ ����

			for (UINT j = 0; j < mNumPatchVertCols; ++j)
			{
				float x = -halfWidth + j * patchWidth; // ���� ����

				patchVertices[i * mNumPatchVertCols + j].Pos = Vector3(x, 0.0f, z);

				patchVertices[i * mNumPatchVertCols + j].Tex.x = j * du;
				patchVertices[i * mNumPatchVertCols + j].Tex.y = i * dv;
			}
		}

		for (UINT i = 0; i < mNumPatchVertRows - 1; ++i)
		{
			for (UINT j = 0; j < mNumPatchVertCols - 1; ++j)
			{
				// ��ġ�� ���δ� ������ �ּ�/�ִ븦 �̸� ����� ���� �������ش�.
				UINT patchID = i * (mNumPatchVertCols - 1) + j;
				patchVertices[i * mNumPatchVertCols + j].BoundsY = mPatchBoundsY[patchID];
			}
		}

		mVertexBuffer.Init(device, patchVertices);
	}

	void Terrain::buildQuadPatchIB(ID3D11Device* device)
	{
		// ������ ��ġ ���¶� Face�� 4���� ������ ���´�. 
		std::vector<UINT> indices(mNumPatchQuadFaces * 4);

		int k = 0;
		for (UINT i = 0; i < mNumPatchVertRows - 1; ++i)
		{
			for (UINT j = 0; j < mNumPatchVertCols - 1; ++j)
			{
				// �� ���� �ΰ�
				indices[k] = i * mNumPatchVertCols + j;
				indices[k + 1] = i * mNumPatchVertCols + j + 1;

				// �Ʒ� ���� �ΰ�
				indices[k + 2] = (i + 1) * mNumPatchVertCols + j;
				indices[k + 3] = (i + 1) * mNumPatchVertCols + j + 1;

				k += 4;
			}
		}

		mIndexBuffer.Init(device, indices);
	}

	void Terrain::buildHeightmapSRV(ID3D11Device* device)
	{
		// �������� ���� Ŭ������ ������ ����

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

		// 16��Ʈ float ����� ����

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
		srvDesc.Texture2D.MipLevels = -1; // �ִ� �Ӹ� ��, -1�� ��� �Ӹ� ���� ǥ��
		HR(device->CreateShaderResourceView(hmapTex, &srvDesc, mHeightMapSRV.GetAddressOf()));

		ReleaseCOM(hmapTex);
	}
}