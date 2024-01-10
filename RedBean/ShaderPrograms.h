#pragma once

#include "ConstantBuffer.h"

namespace builtIn
{
#pragma region ShaderProgramBase
	struct ShaderDesc
	{
		const WCHAR* FilePathOrNull;
		LPCSTR EntryPoint;
		LPCSTR ShaderModel;
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

	class ShaderProgram
	{
	public:
		ShaderProgram() = default;
		~ShaderProgram() = default;

		bool Init(ID3D11Device* device, const ShaderProgramDesc& shaderProgramDesc);

	private:
		D3D11_PRIMITIVE_TOPOLOGY mTopology;
		ComPtr<ID3D11InputLayout> mInputLayout;
		ComPtr<ID3D11VertexShader> mVertexShader;
		ComPtr<ID3D11HullShader> mHullShader;
		ComPtr<ID3D11DomainShader> mDomainShader;
		ComPtr<ID3D11GeometryShader> mGeometryShader;
		ComPtr<ID3D11PixelShader> mPixelShader;
	};

#pragma endregion

	class BasicModel : public ShaderProgram
	{
	public:
		struct PerObject
		{
			Matrix WVP;
			Vector4 CameraPos;
		};

		struct PerLight
		{
			Vector4 LightDirection;
			Vector4 LightColor;
		};

		struct PerTexture
		{
			int bUseDiffuse;
			int bUseNormal;
			int bUseSpecular;
			int bUseAlpha;
		};

	public:
		void Bind(ID3D11DeviceContext* context) {};

		void UpdateSubresource(ID3D11DeviceContext* context, const PerObject& perFrame) {};
		void UpdateSubresource(ID3D11DeviceContext* context, const PerLight& perFrame) {};
		void UpdateSubresource(ID3D11DeviceContext* context, const PerTexture& perFrame) {};

	private:
		directXWrapper::ConstantBuffer<PerObject> mPerObjectCB;
		directXWrapper::ConstantBuffer<PerLight> mPerObjectCB;
		directXWrapper::ConstantBuffer<PerTexture> mPerObjectCB;
	};

	class SkinnedModel : public ShaderProgram
	{
	public:
		struct PerObject
		{
			Matrix WVP;
			Vector4 CameraPos;
		};

		struct BonePalette
		{
			Matrix BoneMat[128];
		};

		struct PerLight
		{
			Vector4 LightDirection;
			Vector4 LightColor;
		};

		struct PerTexture
		{
			int bUseDiffuse;
			int bUseNormal;
			int bUseSpecular;
			int bUseAlpha;
		};
	public:
		void Bind(ID3D11DeviceContext* context);
		void UpdateSubresource(ID3D11DeviceContext* context, PerObject perFrame);
		void UpdateSubresource(ID3D11DeviceContext* context, BonePalette perFrame);
		void UpdateSubresource(ID3D11DeviceContext* context, PerLight perFrame);

	private:
		directXWrapper::ConstantBuffer<PerObject> mPerObjectCB;
		directXWrapper::ConstantBuffer<PerLight> mPerObjectCB;
		directXWrapper::ConstantBuffer<PerTexture> mPerObjectCB;
	};

	class ParticleStreamOut : public ShaderProgram
	{
	};

	class ParticleDraw : public ShaderProgram
	{

	};

	class ShadowMap : public ShaderProgram
	{

	};

	class Sky : public ShaderProgram
	{

	};

	class Ssao : public ShaderProgram
	{

	};

	class SsaoBlur : public ShaderProgram
	{

	};

	class ShaderProgrames
	{
	public:
		static void InitAll(ID3D11Device* device);
		static void DestroyAll();

	public:
		static BasicModel BasicModelProgram;
		static SkinnedModel SkinnedModelProgram;
	};
}