#pragma once

#include "IBindable.h"

namespace bind
{
	class Texture : public IBindable
	{
	public:
		Texture(Graphics& graphics, const std::string& path, UINT slot = 0);
		~Texture() = default;

		// static shared_ptr<Texture> Create(Graphics& graphics, const std::string& path, UINT slot = 0);
		// static string GenerateUID(const std::string& path, UINT slot = 0);

		virtual void Bind(Graphics& graphics)  override;

		string GetUID() const  override;
		bool HasAlpha() const;

	private:
		static UINT CalculateNumberOfMipLevels(UINT width, UINT height);

	protected:
		bool mbHasAlpha = false;
		string mPath;
		ComPtr<ID3D11ShaderResourceView> mpTextureView;

	private:
		unsigned int mSlot;
	};
}
