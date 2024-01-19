#pragma once

#include "IBindable.h"
#include "common.h"
#include "BindableResourceManager.h"

namespace bind
{
	class PixelShader : public IBindable
	{
	public:
		PixelShader(Graphics& graphics, const std::wstring& path)
		{
			ComPtr<ID3DBlob> pBlob;

			D3DHelper::CompileShaderFromFile(path.c_str(), "main", "ps_5_0", pBlob.GetAddressOf());
			// 접두사따라 분기하는 로직
			// D3DReadFileToBlob(path.c_str(), &pBlob);

			GetDevice(graphics)->CreatePixelShader(
				pBlob->GetBufferPointer(),
				pBlob->GetBufferSize(),
				nullptr,
				&mpPixelShader);
		}
		~PixelShader() = default;

		static shared_ptr<PixelShader> Resolve(Graphics& graphics, const std::string& path)
		{
			return ResourceManager::Create<PixelShader>(graphics, path);
		}
		static string GenerateUID(const std::string& path)
		{
			return typeid(PixelShader).name() + "#"s + path;
		}

		void Bind(Graphics& graphics) override
		{
			GetContext(graphics)->PSSetShader(mpPixelShader.Get(), nullptr, 0u);
		}

		string GetUID() const override
		{
			return GenerateUID(mPath);
		}

	protected:
		string mPath;
		ComPtr<ID3D11PixelShader> mpPixelShader;
	};

	class NullPixelShader : public IBindable
	{
	public:
		NullPixelShader(Graphics& graphics)
		{


		}
		~NullPixelShader() = default;

		static std::shared_ptr<NullPixelShader> Resolve(Graphics& graphics)
		{

		}
		static std::string GenerateUID()
		{
			return typeid(NullPixelShader).name();
		}

		virtual void Bind(Graphics& graphics) override
		{
			GetContext(graphics)->PSSetShader(nullptr, nullptr, 0u);
		}

		std::string GetUID() const override
		{
			return GenerateUID();
		}
	};
}