//#pragma once
//
//#include "IBindable.h"
//
//namespace bind
//{
//	class Stencil : public IBindable
//	{
//	public:
//		enum class eMode
//		{
//			Off,
//			Write,
//			Mask
//		};
//
//	public:
//		Stencil(Graphics& graphics, eMode mode)
//			: mMode(mode)
//		{
//			D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
//
//			if (mode == eMode::Write)
//			{
//				dsDesc.DepthEnable = FALSE;
//				dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
//				dsDesc.StencilEnable = TRUE;
//				dsDesc.StencilWriteMask = 0xFF;
//				dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//				dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
//			}
//			else if (mode == eMode::Mask)
//			{
//				dsDesc.DepthEnable = FALSE;
//				dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
//				dsDesc.StencilEnable = TRUE;
//				dsDesc.StencilReadMask = 0xFF;
//				dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
//				dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//			}
//
//			GetDevice(graphics)->CreateDepthStencilState(&dsDesc, &pStencil);
//		}
//		~Stencil() = default;
//
//		static shared_ptr<Stencil> Create(Graphics& graphics, eMode mode);
//		static string GenerateUID(eMode mode);
//
//		virtual void Bind(Graphics& graphics) override;
//
//		string GetUID() const override;
//
//	private:
//		eMode mMode;
//		ComPtr<ID3D11DepthStencilState> pStencil;
//	};
//}
