#pragma once

#include "ConstantBuffers.h"
#include "VertexTypes.h"
#include "RenderStates.h"

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

		ComPtr<ID3DBlob> GetVSByteCode() const { return mVSBytecode; }

	protected:
		ComPtr<ID3DBlob> mVSBytecode = nullptr;
		ComPtr<ID3D11VertexShader> mVertexShader = nullptr;
		ComPtr<ID3D11HullShader> mHullShader = nullptr;
		ComPtr<ID3D11DomainShader> mDomainShader = nullptr;
		ComPtr<ID3D11GeometryShader> mGeometryShader = nullptr;
		ComPtr<ID3D11PixelShader> mPixelShader = nullptr;
	};

#pragma endregion

#pragma region ConcreteShaderProgrmas
	class BasicModel : public ShaderProgram
	{
	public:
		void Bind(ID3D11DeviceContext* context)
		{
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->IASetInputLayout(InputLayouts::PosTexNormalTan);

			context->VSSetShader(mVertexShader.Get(), 0, 0);
			context->PSSetShader(mPixelShader.Get(), 0, 0);

			ID3D11Buffer* vsCBs[] =
			{
				ConstantBuffers::TransformWCB.GetComPtr().Get(),
				ConstantBuffers::TransformVPCB.GetComPtr().Get(),
			};
			context->VSSetConstantBuffers(0, ARRAYSIZE(vsCBs), vsCBs);

			ID3D11Buffer* psCBs[] =
			{
				 ConstantBuffers::FrameCB.GetComPtr().Get(),
				 ConstantBuffers::LegacyMaterialCB.GetComPtr().Get(),
			};
			context->PSSetConstantBuffers(0, ARRAYSIZE(psCBs), psCBs);

			ID3D11SamplerState* psSamplers[] =
			{
				RenderStates::LinearWrapSS.Get()
			};
			context->PSSetSamplers(0, ARRAYSIZE(psSamplers), psSamplers);
		}
	};

	class PBRBasicModel : public ShaderProgram
	{
	public:
		void Bind(ID3D11DeviceContext* context)
		{
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->IASetInputLayout(InputLayouts::PosTexNormalTan);

			context->VSSetShader(mVertexShader.Get(), 0, 0);
			context->PSSetShader(mPixelShader.Get(), 0, 0);

			ID3D11Buffer* vsCBs[] =
			{
				ConstantBuffers::TransformWCB.GetComPtr().Get(),
				ConstantBuffers::TransformVPCB.GetComPtr().Get(),
			};
			context->VSSetConstantBuffers(0, ARRAYSIZE(vsCBs), vsCBs);

			ID3D11Buffer* psCBs[] =
			{
				 ConstantBuffers::FrameCB.GetComPtr().Get(),
				 ConstantBuffers::PBRMaterialCB.GetComPtr().Get(),
			};
			context->PSSetConstantBuffers(0, ARRAYSIZE(psCBs), psCBs);

			ID3D11SamplerState* psSamplers[] =
			{
				RenderStates::AnisotropicWrapSS.Get(),
				RenderStates::LinearClampSS.Get(),
			};
			context->PSSetSamplers(0, ARRAYSIZE(psSamplers), psSamplers);
		}
	};

	class SkinnedModel : public ShaderProgram
	{
	public:
		void Bind(ID3D11DeviceContext* context)
		{
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->IASetInputLayout(InputLayouts::PosTexNormalTan);

			context->VSSetShader(mVertexShader.Get(), 0, 0);
			context->PSSetShader(mPixelShader.Get(), 0, 0);

			ID3D11Buffer* vsCBs[] =
			{
				ConstantBuffers::TransformWCB.GetComPtr().Get(),
				ConstantBuffers::TransformVPCB.GetComPtr().Get(),
				ConstantBuffers::BonePaletteCB.GetComPtr().Get(),
			};
			context->VSSetConstantBuffers(0, ARRAYSIZE(vsCBs), vsCBs);

			ID3D11Buffer* psCBs[] =
			{
				 ConstantBuffers::FrameCB.GetComPtr().Get(),
				 ConstantBuffers::LegacyMaterialCB.GetComPtr().Get(),
			};
			context->PSSetConstantBuffers(0, ARRAYSIZE(psCBs), psCBs);

			ID3D11SamplerState* psSamplers[] =
			{
				RenderStates::LinearWrapSS.Get()
			};
			context->PSSetSamplers(0, ARRAYSIZE(psSamplers), psSamplers);
		}
	};

	class PBRSkinnedModel : public ShaderProgram
	{
	public:
		void Bind(ID3D11DeviceContext* context)
		{
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->IASetInputLayout(InputLayouts::PosTexNormalTan);

			context->VSSetShader(mVertexShader.Get(), 0, 0);
			context->PSSetShader(mPixelShader.Get(), 0, 0);

			ID3D11Buffer* vsCBs[] =
			{
				ConstantBuffers::TransformWCB.GetComPtr().Get(),
				ConstantBuffers::TransformVPCB.GetComPtr().Get(),
				ConstantBuffers::BonePaletteCB.GetComPtr().Get(),
			};
			context->VSSetConstantBuffers(0, ARRAYSIZE(vsCBs), vsCBs);

			ID3D11Buffer* psCBs[] =
			{
				 ConstantBuffers::FrameCB.GetComPtr().Get(),
				 ConstantBuffers::PBRMaterialCB.GetComPtr().Get(),
			};
			context->PSSetConstantBuffers(0, ARRAYSIZE(psCBs), psCBs);

			ID3D11SamplerState* psSamplers[] =
			{
				RenderStates::AnisotropicWrapSS.Get(),
				RenderStates::LinearClampSS.Get(),
			};
			context->PSSetSamplers(0, ARRAYSIZE(psSamplers), psSamplers);
		}
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
#pragma endregion

	class ShaderPrograms
	{
	public:
		static void InitAll(ID3D11Device* device);
		static void DestroyAll();

	public:
		static BasicModel BasicModelProgram;
		static PBRBasicModel PBRBasicModelProgram;
		static SkinnedModel SkinnedModelProgram;
		static PBRSkinnedModel PBRSkinnedModelProgram;
	};
}