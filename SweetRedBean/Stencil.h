#pragma once

#include "IBindable.h"

namespace Bind
{
	class Stencil : public IBindable
	{
	public:
		enum class Mode
		{
			Off,
			Write,
			Mask
		};

	public:
		Stencil(Graphics& graphics, Mode mode);
		virtual ~Stencil() = default;

		static std::shared_ptr<Stencil> Create(Graphics& graphics, Mode mode);
		static std::string GenerateUID(Mode mode);

		void Bind(Graphics& graphics) override;

		std::string GetUID() const override;
	
	private:
		Mode mMode;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mpStencil;
	};
}
