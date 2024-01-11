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

	void OnSize(int width, int height, float fovy, float farZ); // 뷰포트, 후면버퍼, 원평면 재조정
	void SetNormalDepthRenderTarget(ID3D11DepthStencilView* dsv); // 노말맵과 전달한 dsv 바인딩 // 이거 한 후에 다 그려줘야 함
	void ComputeSsao(const Matrix& proj); // Ssao맵 생성
	void BlurAmbientMap(int blurCount); // Ssao맵 가장자리보존 흐리기 적용 

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

	// 화면 전체를 감싸는 정점
	ComPtr<ID3D11Buffer> mScreenQuadVB;
	ComPtr<ID3D11Buffer> mScreenQuadIB;

	// 무작위 표본을 위한 랜덤벡터
	ComPtr<ID3D11ShaderResourceView> mRandomVectorSRV;

	// 법선/깊이 맵과 연결된 뷰들
	ComPtr<ID3D11RenderTargetView> mNormalDepthRTV;
	ComPtr<ID3D11ShaderResourceView> mNormalDepthSRV;

	// ssao맵과 연결된 뷰들, 흐리기를 위해 핑퐁형태로 구현
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
