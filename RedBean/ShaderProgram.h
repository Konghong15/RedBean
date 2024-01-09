#pragma once

namespace directXWrapper
{
	struct ShaderDesc
	{
		const WCHAR* szFileNameOrNull;
		LPCSTR szEntryPoint;
		LPCSTR szShaderModel;
	};

	struct ShaderProgramDesc
	{
		D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		const std::vector<D3D11_INPUT_ELEMENT_DESC>* InputLayoutDescOrNull = nullptr;
		ShaderDesc VSDesc = { nullptr, "main", "vs_5_0" };
		ShaderDesc HSDesc = { nullptr, "main", "hs_5_0" };
		ShaderDesc DSDesc = { nullptr, "main", "ds_5_0" };
		ShaderDesc GSDesc = { nullptr, "main", "gs_5_0" };
		ShaderDesc PSDesc = { nullptr, "main", "ps_5_0" };
	};

	// 게임 시작 시에 이 넘을 빌드하고 하나로 쓸 수 있게 둔다.
	class ShaderProgram
	{
	public:
		ShaderProgram() = default;
		~ShaderProgram() = default;

		bool Init(ID3D11Device* device, const ShaderProgramDesc& shaderProgramDesc);
		void Bind(ID3D11DeviceContext* context);

	private:
		D3D11_PRIMITIVE_TOPOLOGY mTopology;
		ComPtr<ID3D11InputLayout> mInputLayout;
		ComPtr<ID3D11VertexShader> mVertexShader;
		ComPtr<ID3D11HullShader> mHullShader;
		ComPtr<ID3D11DomainShader> mDomainShader;
		ComPtr<ID3D11GeometryShader> mGeometryShader;
		ComPtr<ID3D11PixelShader> mPixelShader;
	};
}