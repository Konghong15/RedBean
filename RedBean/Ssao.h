#pragma once

#include "ShaderPrograms.h"
#include "ConstantBuffer.h"

class Ssao
{
public:
	struct SsaoPerFrame
	{
		Matrix ViewToTexSpace; // Proj*Texture
		Vector4 OffsetVectors[14];
		Vector4 FrustumCorners[4];
	};

	struct SsaoBlurPerFrame
	{
		float TexelWidth;
		float TexelHeight;
		int bIsHorizontalBlur;
		int unused[1];
	};

public:
	Ssao(ID3D11Device* device, ID3D11DeviceContext* dc, int width, int height, float fovy, float farZ);
	~Ssao() = default;
	Ssao(const Ssao& rhs) = delete;
	Ssao& operator=(const Ssao& rhs) = delete;

	void OnSize(int width, int height, float fovy, float farZ); // ����Ʈ, �ĸ����, ����� ������
	void SetNormalDepthRenderTarget(ID3D11DepthStencilView* dsv); // �븻�ʰ� ������ dsv ���ε� // �̰� �� �Ŀ� �� �׷���� ��
	void ComputeSsao(const Matrix& proj); // Ssao�� ����
	void BlurAmbientMap(int blurCount); // Ssao�� �����ڸ����� �帮�� ���� 

	ComPtr<ID3D11ShaderResourceView> GetNormalDepthSRV() { return mNormalDepthSRV; }
	ComPtr<ID3D11ShaderResourceView> GetAmbientSRV() { return mAmbientSRV0; }

private:
	void BlurAmbientMap(ID3D11ShaderResourceView* inputSRV, ID3D11RenderTargetView* outputRTV, bool bIsHorzBlur);
	void BuildFrustumFarCorners(float fovy, float farZ);
	void BuildFullScreenQuad();
	void BuildTextureViews();
	void BuildRandomVectorTexture();
	void BuildOffsetVectors();

private:
	enum { SAMPLE_RAY_COUNT = 14 };

	ComPtr<ID3D11Device> md3dDevice;
	ComPtr<ID3D11DeviceContext> mDC;

	// ȭ�� ��ü�� ���δ� ����
	ComPtr<ID3D11Buffer> mScreenQuadVB;
	ComPtr<ID3D11Buffer> mScreenQuadIB;

	// ������ ǥ���� ���� ��������
	ComPtr<ID3D11ShaderResourceView> mRandomVectorSRV;

	// ����/���� �ʰ� ����� ���
	ComPtr<ID3D11RenderTargetView> mNormalDepthRTV;
	ComPtr<ID3D11ShaderResourceView> mNormalDepthSRV;

	// ssao�ʰ� ����� ���, �帮�⸦ ���� �������·� ����
	ComPtr<ID3D11RenderTargetView> mAmbientRTV0;
	ComPtr<ID3D11ShaderResourceView> mAmbientSRV0;
	ComPtr<ID3D11RenderTargetView> mAmbientRTV1;
	ComPtr<ID3D11ShaderResourceView> mAmbientSRV1;

	UINT mRenderTargetWidth;
	UINT mRenderTargetHeight;
	Vector4 mFrustumFarCorner[4];
	Vector4 mOffsets[SAMPLE_RAY_COUNT];
	D3D11_VIEWPORT mAmbientMapViewport;
};
