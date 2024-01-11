//#include "pch.h"
//
//#include "Ssao.h"
//#include "RenderStates.h"
//
//Ssao::Ssao(ID3D11Device* device, ID3D11DeviceContext* dc, int width, int height, float fovy, float farZ)
//	: md3dDevice(device)
//	, mDC(dc)
//	, mScreenQuadVB(nullptr)
//	, mScreenQuadIB(nullptr)
//	, mRandomVectorSRV(nullptr)
//	, mNormalDepthRTV(nullptr)
//	, mNormalDepthSRV(nullptr)
//	, mAmbientRTV0(nullptr)
//	, mAmbientSRV0(nullptr)
//	, mAmbientRTV1(nullptr)
//	, mAmbientSRV1(nullptr)
//
//{
//	OnSize(width, height, fovy, farZ);
//
//	BuildFullScreenQuad();
//	BuildOffsetVectors();
//	BuildRandomVectorTexture();
//
//	directXWrapper::ShaderProgramDesc shaderDesc;
//	shaderDesc.VSDesc.FilePathOrNull = L"NormalMapVS.hlsl";
//	shaderDesc.PSDesc.FilePathOrNull = L"NormalMapPS.hlsl";
//	mNormalSP.Init(device, shaderDesc);
//
//	shaderDesc.VSDesc.FilePathOrNull = L"SsaoBlurVS.hlsl";
//	shaderDesc.PSDesc.FilePathOrNull = L"SsaoBlurPS.hlsl";
//	mSsaoBlurSP.Init(device, shaderDesc);
//
//	shaderDesc.VSDesc.FilePathOrNull = L"SsaoVS.hlsl";
//	shaderDesc.PSDesc.FilePathOrNull = L"SsaoPS.hlsl";
//	mSsaoSP.Init(device, shaderDesc);
//}
//
//void Ssao::OnSize(int width, int height, float fovy, float farZ)
//{
//	mRenderTargetWidth = width;
//	mRenderTargetHeight = height;
//
//	// ����Ʈ �缳��
//	mAmbientMapViewport.TopLeftX = 0.0f;
//	mAmbientMapViewport.TopLeftY = 0.0f;
//	mAmbientMapViewport.Width = width / 2.0f;
//	mAmbientMapViewport.Height = height / 2.0f;
//	mAmbientMapViewport.MinDepth = 0.0f;
//	mAmbientMapViewport.MaxDepth = 1.0f;
//
//	// ������ �ʺ� ���� ����
//	BuildFrustumFarCorners(fovy, farZ);
//
//	// �ؽ�ó �����
//	BuildTextureViews();
//}
//
//void Ssao::SetNormalDepthRenderTarget(ID3D11DepthStencilView* dsv)
//{
//	ID3D11RenderTargetView* renderTargets[1] = { mNormalDepthRTV.Get() };
//	mDC->OMSetRenderTargets(1, renderTargets, dsv);
//
//	// �븻�� ���� 0, 0, -1�� �ʱ�ȭ�ϰ� ���̰��� ū ������ �ʱ�ȭ�Ѵ�.
//	float clearColor[] = { 0.0f, 0.0f, -1.0f, 1e5f };
//	mDC->ClearRenderTargetView(renderTargets[0], clearColor);
//}
//
//void Ssao::ComputeSsao(const Matrix& proj)
//{
//	// ssao rtv ���ε�
//	ID3D11RenderTargetView* renderTargets[1] = { mAmbientRTV0.Get() };
//	mDC->OMSetRenderTargets(1, renderTargets, 0);
//	mDC->ClearRenderTargetView(renderTargets[0], common::Black);
//	mDC->RSSetViewports(1, &mAmbientMapViewport);
//
//	// ��� ���� ����, ���ε�
//	{
//		// ndc -> texture space
//		static const Matrix toTexture(
//			0.5f, 0.0f, 0.0f, 0.0f,
//			0.0f, -0.5f, 0.0f, 0.0f,
//			0.0f, 0.0f, 1.0f, 0.0f,
//			0.5f, 0.5f, 0.0f, 1.0f);
//
//		XMMATRIX PT = proj * toTexture;
//
//		SsaoPerFrame ssaoPerFrame;
//		ssaoPerFrame.ViewToTexSpace = PT;
//		memcpy(ssaoPerFrame.OffsetVectors, mOffsets, sizeof(mOffsets));
//		memcpy(ssaoPerFrame.FrustumCorners, mFrustumFarCorner, sizeof(mFrustumFarCorner));
//
//		ComPtr<ID3D11Buffer> cb = mSsaoPerFrameCB.GetComPtr();
//		mDC->UpdateSubresource(cb.Get(), 0, NULL, &ssaoPerFrame, 0, 0);
//		mDC->VSSetConstantBuffers(0, 1, cb.GetAddressOf());
//		mDC->PSSetConstantBuffers(0, 1, cb.GetAddressOf());
//	}
//
//	// ���÷� ������Ʈ, SRV ���ε�
//	{
//		ID3D11ShaderResourceView* SRVs[] = { mNormalDepthSRV.Get(), mRandomVectorSRV.Get() };
//		mDC->PSSetShaderResources(0, 2, SRVs);
//		ID3D11SamplerState* SSs[] = { directXWrapper::RenderStates::PointBoaderSS.Get(), directXWrapper::RenderStates::LinearWrapSS.Get() };
//		mDC->PSSetSamplers(0, 2, SSs);
//	}
//
//	// ���̴� ���α׷� ���ε�
//	mSsaoSP.Bind(mDC.Get());
//
//	// ����ȭ�� �簢�� �׸���
//	UINT stride = sizeof(common::VertexTextureNormalData);
//	UINT offset = 0;
//	mDC->IASetVertexBuffers(0, 1, mScreenQuadVB.GetAddressOf(), &stride, &offset);
//	mDC->IASetIndexBuffer(mScreenQuadIB.Get(), DXGI_FORMAT_R16_UINT, 0);
//	mDC->DrawIndexed(6, 0, 0);
//}
//
//void Ssao::BlurAmbientMap(int blurCount)
//{
//	mSsaoBlurSP.Bind(mDC.Get());
//
//	for (int i = 0; i < blurCount; ++i)
//	{
//		// ���� ������� ���� �帮�� + ���� �帮�� �ݺ�
//		BlurAmbientMap(mAmbientSRV0.Get(), mAmbientRTV1.Get(), true);
//		BlurAmbientMap(mAmbientSRV1.Get(), mAmbientRTV0.Get(), false);
//	}
//}
//
//void Ssao::BlurAmbientMap(ID3D11ShaderResourceView* inputSRV, ID3D11RenderTargetView* outputRTV, bool bIsHorzBlur)
//{
//	// ����Ÿ�� ���ε�
//	ID3D11RenderTargetView* renderTargets[1] = { outputRTV };
//	mDC->OMSetRenderTargets(1, renderTargets, 0);
//	mDC->ClearRenderTargetView(outputRTV, common::Black);
//	mDC->RSSetViewports(1, &mAmbientMapViewport);
//
//	// SRV ���ε�
//	mDC->PSSetShaderResources(1, 1, &inputSRV);
//
//	// ������� ������Ʈ
//	SsaoBlurPerFrame ssaoBlurPerFrame;
//	ssaoBlurPerFrame.TexelWidth = 1.f / mAmbientMapViewport.Width;
//	ssaoBlurPerFrame.TexelHeight = 1.f / mAmbientMapViewport.Height;
//	ssaoBlurPerFrame.bIsHorizontalBlur = bIsHorzBlur;
//	ComPtr<ID3D11Buffer> cb = mSsaoBlurPerFrameCB.GetComPtr();
//	mDC->UpdateSubresource(cb.Get(), 0, NULL, &ssaoBlurPerFrame, 0, 0);
//	mDC->PSSetConstantBuffers(0, 1, cb.GetAddressOf());
//
//	// ����ȭ�� �簢�� ���ε�
//	UINT stride = sizeof(common::VertexTextureNormalData);
//	UINT offset = 0;
//	mDC->IASetVertexBuffers(0, 1, mScreenQuadVB.GetAddressOf(), &stride, &offset);
//	mDC->IASetIndexBuffer(mScreenQuadIB.Get(), DXGI_FORMAT_R16_UINT, 0);
//	mDC->DrawIndexed(6, 0, 0);
//}
//
//void Ssao::BuildFrustumFarCorners(float fovy, float farZ)
//{
//	// ������ ��� ������ �������� ���� ����
//	float aspect = (float)mRenderTargetWidth / (float)mRenderTargetHeight;
//	float halfHeight = farZ * tanf(0.5f * fovy);
//	float halfWidth = aspect * halfHeight;
//
//	mFrustumFarCorner[0] = { halfWidth, -halfHeight, farZ, 0.0f };
//	mFrustumFarCorner[1] = { halfWidth, +halfHeight, farZ, 0.0f };
//	mFrustumFarCorner[2] = { halfWidth, +halfHeight, farZ, 0.0f };
//	mFrustumFarCorner[3] = { halfWidth, -halfHeight, farZ, 0.0f };
//}
//
//void Ssao::BuildFullScreenQuad()
//{
//	common::VertexTextureNormalData v[4];
//
//	// ȭ���� ���δ� NDC ��ǥ�� �簢�� ����
//	v[0].Position = { -1.0f, -1.0f, 0.0f };
//	v[1].Position = { -1.0f, +1.0f, 0.0f };
//	v[2].Position = { +1.0f, +1.0f, 0.0f };
//	v[3].Position = { +1.0f, -1.0f, 0.0f };
//
//	v[0].Normal = { 0.0f, 0.0f, 0.0f };
//	v[1].Normal = { 1.0f, 0.0f, 0.0f };
//	v[2].Normal = { 2.0f, 0.0f, 0.0f };
//	v[3].Normal = { 3.0f, 0.0f, 0.0f };
//
//	v[0].UV = { 0.0f, 1.0f };
//	v[1].UV = { 0.0f, 0.0f };
//	v[2].UV = { 1.0f, 0.0f };
//	v[3].UV = { 1.0f, 1.0f };
//
//	D3D11_BUFFER_DESC vbd;
//	vbd.Usage = D3D11_USAGE_IMMUTABLE;
//	vbd.ByteWidth = sizeof(common::VertexTextureNormalData) * 4;
//	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vbd.CPUAccessFlags = 0;
//	vbd.MiscFlags = 0;
//	vbd.StructureByteStride = 0;
//
//	D3D11_SUBRESOURCE_DATA vinitData;
//	vinitData.pSysMem = v;
//
//	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mScreenQuadVB));
//
//	USHORT indices[6] =
//	{
//		0, 1, 2,
//		0, 2, 3
//	};
//
//	D3D11_BUFFER_DESC ibd;
//	ibd.Usage = D3D11_USAGE_IMMUTABLE;
//	ibd.ByteWidth = sizeof(USHORT) * 6;
//	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	ibd.CPUAccessFlags = 0;
//	ibd.StructureByteStride = 0;
//	ibd.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA iinitData;
//	iinitData.pSysMem = indices;
//
//	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mScreenQuadIB));
//}
//
//void Ssao::BuildTextureViews()
//{
//	D3D11_TEXTURE2D_DESC texDesc;
//	texDesc.Width = mRenderTargetWidth;
//	texDesc.Height = mRenderTargetHeight;
//	texDesc.MipLevels = 1;
//	texDesc.ArraySize = 1;
//	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
//	texDesc.SampleDesc.Count = 1;
//	texDesc.SampleDesc.Quality = 0;
//	texDesc.Usage = D3D11_USAGE_DEFAULT;
//	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
//	texDesc.CPUAccessFlags = 0;
//	texDesc.MiscFlags = 0;
//
//	// ȭ�� ũ��� �븻�� view ����
//	ID3D11Texture2D* normalDepthTex = 0;
//	HR(md3dDevice->CreateTexture2D(&texDesc, 0, &normalDepthTex));
//	HR(md3dDevice->CreateShaderResourceView(normalDepthTex, 0, &mNormalDepthSRV));
//	HR(md3dDevice->CreateRenderTargetView(normalDepthTex, 0, &mNormalDepthRTV));
//	ReleaseCOM(normalDepthTex);
//
//	// 1/4 ũ��� ssao�ʰ� view ����
//	texDesc.Width = mRenderTargetWidth / 2;
//	texDesc.Height = mRenderTargetHeight / 2;
//	texDesc.Format = DXGI_FORMAT_R16_FLOAT; // 16��Ʈ ȸ���� �̹���
//
//	ID3D11Texture2D* ambientTex0 = 0;
//	HR(md3dDevice->CreateTexture2D(&texDesc, 0, &ambientTex0));
//	HR(md3dDevice->CreateShaderResourceView(ambientTex0, 0, &mAmbientSRV0));
//	HR(md3dDevice->CreateRenderTargetView(ambientTex0, 0, &mAmbientRTV0));
//
//	ID3D11Texture2D* ambientTex1 = 0;
//	HR(md3dDevice->CreateTexture2D(&texDesc, 0, &ambientTex1));
//	HR(md3dDevice->CreateShaderResourceView(ambientTex1, 0, &mAmbientSRV1));
//	HR(md3dDevice->CreateRenderTargetView(ambientTex1, 0, &mAmbientRTV1));
//
//	ReleaseCOM(ambientTex0);
//	ReleaseCOM(ambientTex1);
//}
//
//void Ssao::BuildRandomVectorTexture()
//{
//	// ǥ���� �ݻ��ų ���� ���� ����
//
//	D3D11_TEXTURE2D_DESC texDesc;
//	texDesc.Width = 256;
//	texDesc.Height = 256;
//	texDesc.MipLevels = 1;
//	texDesc.ArraySize = 1;
//	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	texDesc.SampleDesc.Count = 1;
//	texDesc.SampleDesc.Quality = 0;
//	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
//	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//	texDesc.CPUAccessFlags = 0;
//	texDesc.MiscFlags = 0;
//
//	DirectX::SimpleMath::Color color[256 * 256];
//	for (int i = 0; i < 256; ++i)
//	{
//		for (int j = 0; j < 256; ++j)
//		{
//			XMFLOAT3 v(common::MathHelper::RandF(), common::MathHelper::RandF(), common::MathHelper::RandF());
//
//			color[i * 256 + j] = { v.x, v.y, v.z, 0.0f };
//		}
//	}
//
//	D3D11_SUBRESOURCE_DATA initData = { 0 };
//	initData.SysMemPitch = 256 * sizeof(DirectX::SimpleMath::Color);
//	initData.pSysMem = color;
//
//	ID3D11Texture2D* tex = 0;
//	HR(md3dDevice->CreateTexture2D(&texDesc, &initData, &tex));
//	HR(md3dDevice->CreateShaderResourceView(tex, 0, &mRandomVectorSRV));
//
//	ReleaseCOM(tex);
//}
//
//void Ssao::BuildOffsetVectors()
//{
//	// ������ �������� ã������ �� ������ ���͸� �����.
//
//	// 8 cube corners
//	mOffsets[0] = { +1.0f, +1.0f, +1.0f, 0.0f };
//	mOffsets[1] = { -1.0f, -1.0f, -1.0f, 0.0f };
//
//	mOffsets[2] = { -1.0f, +1.0f, +1.0f, 0.0f };
//	mOffsets[3] = { +1.0f, -1.0f, -1.0f, 0.0f };
//
//	mOffsets[4] = { +1.0f, +1.0f, -1.0f, 0.0f };
//	mOffsets[5] = { -1.0f, -1.0f, +1.0f, 0.0f };
//
//	mOffsets[6] = { -1.0f, +1.0f, -1.0f, 0.0f };
//	mOffsets[7] = { +1.0f, -1.0f, +1.0f, 0.0f };
//
//	// 6 centers of cube faces
//	mOffsets[8] = { -1.0f, 0.0f, 0.0f, 0.0f };
//	mOffsets[9] = { +1.0f, 0.0f, 0.0f, 0.0f };
//
//	mOffsets[10] = { 0.0f, -1.0f, 0.0f, 0.0f };
//	mOffsets[11] = { 0.0f, +1.0f, 0.0f, 0.0f };
//
//	mOffsets[12] = { 0.0f, 0.0f, -1.0f, 0.0f };
//	mOffsets[13] = { 0.0f, 0.0f, +1.0f, 0.0f };
//
//	for (int i = 0; i < SAMPLE_RAY_COUNT; ++i)
//	{
//		// Create random lengths in [0.25, 1.0].
//		mOffsets[i].Normalize();
//		mOffsets[i] *= common::MathHelper::RandF(0.25f, 1.0f);
//	}
//}