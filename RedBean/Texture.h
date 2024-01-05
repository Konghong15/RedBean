#pragma once

#include "IBindable.h"

namespace directXWrapper
{
	// 바인딩 인터페이스를 만드는 게 맞나? 그냥 각자 담고 템플릿으로 다루는 게 더 안전해보이긴 하네
	class Texture : public interfaceClass::IBindable
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		bool Init(ID3D11Device* device, const wstring& path);
		bool Init(ID3D11Device* device, const std::vector<wstring>& paths);

		virtual void Bind(ID3D11DeviceContext* context) override; // 바인딩 정보 미리 정의해놓고 요거 호출하면 알아서 바인딩 되도록 하면 좋을듯

		ComPtr<ID3D11ShaderResourceView> GetComPtr() { return mSRV; }
		const Vector2& GetSize() const { return mSize; }
		const ScratchImage& GetScratchImage() const { return mScratchImage; }

	private:
		ComPtr<ID3D11ShaderResourceView> mSRV;
		Vector2 mSize;
		ScratchImage mScratchImage;

		// 바인딩 정보
	};
}