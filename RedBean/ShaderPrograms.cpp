#include "pch.h"

#include "ShaderPrograms.h"

namespace builtIn
{
	bool ShaderProgram::Init(ID3D11Device* device, const ShaderProgramDesc& shaderProgramDesc)
	{
		mTopology = shaderProgramDesc.Topology;

		ComPtr<ID3DBlob> vsBytecode;

		// Create VertexShader
		if (shaderProgramDesc.VSDesc.FilePathOrNull != nullptr)
		{
			const auto& vsDesc = shaderProgramDesc.VSDesc;

			HR(common::D3DHelper::CompileShaderFromFile(vsDesc.FilePathOrNull,
				vsDesc.EntryPoint,
				vsDesc.ShaderModel,
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
		if (shaderProgramDesc.HSDesc.FilePathOrNull != nullptr)
		{
			const auto& hsDesc = shaderProgramDesc.HSDesc;

			ComPtr<ID3DBlob> hsBytecode;

			HR(common::D3DHelper::CompileShaderFromFile(hsDesc.FilePathOrNull,
				hsDesc.EntryPoint,
				hsDesc.ShaderModel,
				hsBytecode.GetAddressOf()));

			HR(device->CreateHullShader(hsBytecode->GetBufferPointer(),
				hsBytecode->GetBufferSize(),
				nullptr,
				mHullShader.GetAddressOf()));
		}

		// Create Domain Shader
		if (shaderProgramDesc.DSDesc.FilePathOrNull != nullptr)
		{
			const auto& dsDesc = shaderProgramDesc.DSDesc;

			ComPtr<ID3DBlob> dsBytecode;

			HR(common::D3DHelper::CompileShaderFromFile(dsDesc.FilePathOrNull,
				dsDesc.EntryPoint,
				dsDesc.ShaderModel,
				dsBytecode.GetAddressOf()));

			HR(device->CreateDomainShader(dsBytecode->GetBufferPointer(),
				dsBytecode->GetBufferSize(),
				nullptr,
				mDomainShader.GetAddressOf()));
		}

		// Create Geometry Shader
		if (shaderProgramDesc.GSDesc.FilePathOrNull != nullptr)
		{
			const auto& gsDesc = shaderProgramDesc.GSDesc;

			ComPtr<ID3DBlob> gsBytecode;

			HR(common::D3DHelper::CompileShaderFromFile(gsDesc.FilePathOrNull,
				gsDesc.EntryPoint,
				gsDesc.ShaderModel,
				gsBytecode.GetAddressOf()));

			HR(device->CreateGeometryShader(gsBytecode->GetBufferPointer(),
				gsBytecode->GetBufferSize(),
				nullptr,
				mGeometryShader.GetAddressOf()));
		}

		// Create Pixel Shader
		if (shaderProgramDesc.PSDesc.FilePathOrNull != nullptr)
		{
			const auto& psDesc = shaderProgramDesc.PSDesc;

			ComPtr<ID3DBlob> psBytecode;

			HR(common::D3DHelper::CompileShaderFromFile(psDesc.FilePathOrNull,
				psDesc.EntryPoint,
				psDesc.ShaderModel,
				psBytecode.GetAddressOf()));

			HR(device->CreatePixelShader(psBytecode->GetBufferPointer(),
				psBytecode->GetBufferSize(),
				nullptr,
				mPixelShader.GetAddressOf()));
		}

		return true;
	}

	BasicModel ShaderPrograms::BasicModelProgram;
	SkinnedModel ShaderPrograms::SkinnedModelProgram;

	void ShaderPrograms::InitAll(ID3D11Device* device)
	{
		// basic
		{
			ShaderProgramDesc basicDesc;
			basicDesc.VSDesc.FilePathOrNull = L"../RedBean/BasicModelVS.hlsl";
			basicDesc.PSDesc.FilePathOrNull = L"../RedBean/ModelPS.hlsl";
			BasicModelProgram.Init(device, basicDesc);
		}

		// skinned
		{
			ShaderProgramDesc skinnedDesc;
			skinnedDesc.VSDesc.FilePathOrNull = L"../RedBean/SkinnedModelVS.hlsl";
			skinnedDesc.PSDesc.FilePathOrNull = L"../RedBean/ModelPS.hlsl";
			SkinnedModelProgram.Init(device, skinnedDesc);
		}
	}

	void ShaderPrograms::DestroyAll()
	{

	}
}