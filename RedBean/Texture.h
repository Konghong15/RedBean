#pragma once

#include "IBindable.h"

namespace directXWrapper
{
	// ���ε� �������̽��� ����� �� �³�? �׳� ���� ��� ���ø����� �ٷ�� �� �� �����غ��̱� �ϳ�
	class Texture : public interfaceClass::IBindable
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		bool Init(ID3D11Device* device, const wstring& path);
		bool Init(ID3D11Device* device, const std::vector<wstring>& paths);

		virtual void Bind(ID3D11DeviceContext* context) override; // ���ε� ���� �̸� �����س��� ��� ȣ���ϸ� �˾Ƽ� ���ε� �ǵ��� �ϸ� ������

		ComPtr<ID3D11ShaderResourceView> GetComPtr() { return mSRV; }
		const Vector2& GetSize() const { return mSize; }
		const ScratchImage& GetScratchImage() const { return mScratchImage; }

	private:
		ComPtr<ID3D11ShaderResourceView> mSRV;
		Vector2 mSize;
		ScratchImage mScratchImage;

		// ���ε� ����
	};
}