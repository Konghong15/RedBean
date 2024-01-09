#pragma once

namespace directXWrapper
{
	// ��� : �̱������� ����� �� ��������

	class RenderStates
	{
	public:
		static void Init(ID3D11Device* device);

	private:
		static void initSamplerState(ID3D11Device* device);
		static void initRasterizerState(ID3D11Device* device);
		static void initBlendState(ID3D11Device* device);
		static void initDepthStencilState(ID3D11Device* device);

	public:
		// SamplerState
		static ComPtr<ID3D11SamplerState> LinearWrapSS;
		static ComPtr<ID3D11SamplerState> PointClampSS;
		static ComPtr<ID3D11SamplerState> PointBoaderSS;
		static ComPtr<ID3D11SamplerState> PointWrapSS;

		// RasterizerState
		static ComPtr<ID3D11RasterizerState> WireFrameRS; // ���̾����������� ������
		static ComPtr<ID3D11RasterizerState> NoCullRS; // �ĸ����Ÿ� ���� ����
		static ComPtr<ID3D11RasterizerState> CullClockwiseRS; // �ݽð������ �������� ����
		static ComPtr<ID3D11RasterizerState> DepthRS;

		// BlendState
		static ComPtr<ID3D11BlendState> AlphaToCoverageBS; // ����������
		static ComPtr<ID3D11BlendState> TransparentBS; // ���� �ȼ��� ���İ����� ȥ���ϰڴ�.
		static ComPtr<ID3D11BlendState> NoRenderTargetWritesBS; // �������� ȥ���� ���� �ʴ´�.
		static ComPtr<ID3D11BlendState> AdditiveBlending; // �������� ȥ���� ���� �ʴ´�.

		// DepthStecilState
		static ComPtr<ID3D11DepthStencilState> MarkMirrorDSS; // ���� ���� on, ���ٽ� ����/���� on(�׻� ��/��� ��� �� ���ٽ� ���ذ����� ����)
		static ComPtr<ID3D11DepthStencilState> DrawReflectionDSS; // ���� ����/���� on, ���ٽ� ���� on (���� �� ��)
		static ComPtr<ID3D11DepthStencilState> NoDoubleBlendDSS; // ���� ����/���� on, ���ٽ� ����/���� on(���� �� ��/��� ��� �� increase ����)
		static ComPtr<ID3D11DepthStencilState> LessEqualDSS; // ���� ����/���� on (���̰� ���Ƶ� ���)
		static ComPtr<ID3D11DepthStencilState> DisableDepthDSS;
		static ComPtr<ID3D11DepthStencilState> NoDepthWrites;
		static ComPtr<ID3D11DepthStencilState> EqualsDSS;

	private:
		static bool mbIsInit;
	};
}