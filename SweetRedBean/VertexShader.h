#pragma once

#include "IBindable.h"
#include "common.h"

namespace bind
{
	class VertexShader : public IBindable
	{
	public:
		inline VertexShader(Graphics& graphics, const wstring& path);
		~VertexShader() = default;

		// static shared_ptr<VertexShader> Create(Graphics& grapics, const string path);
		// static string GenerateUID(const string& path);

		inline virtual void Bind(Graphics& grapics) override;

		inline ID3D10Blob* GetBytecode() const;
		// string GetUID() const override;

	private:
		// string mPath;
		ComPtr<ID3D10Blob> mpBytecodeBlob;
		ComPtr<ID3D11VertexShader> mpVertexShader;
	};

#pragma region inline
	VertexShader::VertexShader(Graphics& graphics, const wstring& path)
	{
		D3DHelper::CompileShaderFromFile(path.c_str(), "main", "vs_5_0", mpBytecodeBlob.GetAddressOf());

		// D3DReadFileToBlob(path.c_str(), &mpBytecodeBlob);

		GetDevice(graphics)->CreateVertexShader(
			mpBytecodeBlob->GetBufferPointer(),
			mpBytecodeBlob->GetBufferSize(),
			nullptr,
			&mpVertexShader
		);
	}

	void VertexShader::Bind(Graphics& grapics)
	{
		GetContext(grapics)->VSSetShader(mpVertexShader.Get(), nullptr, 0u);
	}

	ID3D10Blob* VertexShader::GetBytecode() const
	{
		return mpBytecodeBlob.Get();
	}
#pragma endregion
}