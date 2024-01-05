#include "pch.h"

#include "ShaderProgram.h"

namespace directXWrapper
{
	bool ShaderProgram::Init(ID3D11Device* device, const ShaderProgramDesc& shaderProgramDesc)
	{
		mTopology = shaderProgramDesc.Topology;

		ComPtr<ID3DBlob> vsBytecode;

		// Create VertexShader
		if (shaderProgramDesc.VSDesc.szFileNameOrNull != nullptr)
		{
			const auto& vsDesc = shaderProgramDesc.VSDesc;

			HR(common::D3DHelper::CompileShaderFromFile(vsDesc.szFileNameOrNull,
				vsDesc.szEntryPoint,
				vsDesc.szShaderModel,
				vsBytecode.GetAddressOf()));

			HR(device->CreateVertexShader(vsBytecode->GetBufferPointer(),
				vsBytecode->GetBufferSize(),
				nullptr,
				mVertexShader.GetAddressOf()));
		}

		// Create InputLayout
		if (shaderProgramDesc.InputLayoutDescOrNull != nullptr)
		{
			auto* inputLayoutDesc = shaderProgramDesc.InputLayoutDescOrNull;

			assert(vsBytecode.Get() != nullptr);

			HR(device->CreateInputLayout(inputLayoutDesc->data(),
				(UINT)inputLayoutDesc->size(),
				vsBytecode->GetBufferPointer(),
				vsBytecode->GetBufferSize(),
				mInputLayout.GetAddressOf()));
		}


		// Create Hull Shader
		if (shaderProgramDesc.HSDesc.szFileNameOrNull != nullptr)
		{
			const auto& hsDesc = shaderProgramDesc.HSDesc;

			ComPtr<ID3DBlob> hsBytecode;

			HR(common::D3DHelper::CompileShaderFromFile(hsDesc.szFileNameOrNull,
				hsDesc.szEntryPoint,
				hsDesc.szShaderModel,
				hsBytecode.GetAddressOf()));

			HR(device->CreateHullShader(hsBytecode->GetBufferPointer(),
				hsBytecode->GetBufferSize(),
				nullptr,
				mHullShader.GetAddressOf()));
		}

		// Create Domain Shader
		if (shaderProgramDesc.DSDesc.szFileNameOrNull != nullptr)
		{
			const auto& dsDesc = shaderProgramDesc.DSDesc;

			ComPtr<ID3DBlob> dsBytecode;

			HR(common::D3DHelper::CompileShaderFromFile(dsDesc.szFileNameOrNull,
				dsDesc.szEntryPoint,
				dsDesc.szShaderModel,
				dsBytecode.GetAddressOf()));

			HR(device->CreateDomainShader(dsBytecode->GetBufferPointer(),
				dsBytecode->GetBufferSize(),
				nullptr,
				mDomainShader.GetAddressOf()));
		}

		// Create Geometry Shader
		if (shaderProgramDesc.GSDesc.szFileNameOrNull != nullptr)
		{
			const auto& gsDesc = shaderProgramDesc.GSDesc;

			ComPtr<ID3DBlob> gsBytecode;

			HR(common::D3DHelper::CompileShaderFromFile(gsDesc.szFileNameOrNull,
				gsDesc.szEntryPoint,
				gsDesc.szShaderModel,
				gsBytecode.GetAddressOf()));

			HR(device->CreateGeometryShader(gsBytecode->GetBufferPointer(),
				gsBytecode->GetBufferSize(),
				nullptr,
				mGeometryShader.GetAddressOf()));
		}

		// Create Pixel Shader
		if (shaderProgramDesc.PSDesc.szFileNameOrNull != nullptr)
		{
			const auto& psDesc = shaderProgramDesc.PSDesc;

			ComPtr<ID3DBlob> psBytecode;

			HR(common::D3DHelper::CompileShaderFromFile(psDesc.szFileNameOrNull,
				psDesc.szEntryPoint,
				psDesc.szShaderModel,
				psBytecode.GetAddressOf()));

			HR(device->CreatePixelShader(psBytecode->GetBufferPointer(),
				psBytecode->GetBufferSize(),
				nullptr,
				mPixelShader.GetAddressOf()));
		}

		return true;
	}

	void ShaderProgram::Bind(ID3D11DeviceContext* context)
	{
		context->IASetPrimitiveTopology(mTopology);

		if (mInputLayout.Get() != nullptr)
		{
			context->IASetInputLayout(mInputLayout.Get());
		}

		if (mVertexShader.Get() != nullptr)
		{
			context->VSSetShader(mVertexShader.Get(), nullptr, 0);
		}

		if (mPixelShader.Get() != nullptr)
		{
			context->PSSetShader(mPixelShader.Get(), nullptr, 0);
		}
	}
}