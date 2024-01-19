//#pragma once
//
//#include "IBindable.h"
//
//namespace bind
//{
//	class Rasterizer : public IBindable
//	{
//	public:
//		Rasterizer(Graphics& graphics, bool twoSided);
//		~Rasterizer() = default;
//
//		static shared_ptr<Rasterizer> Resolve(Graphics& graphics, bool twoSided);
//		static string GenerateUID(bool twoSided);
//
//		virtual void Bind(Graphics& graphics)  override;
//
//		string GetUID() const  override;
//
//	protected:
//		ComPtr<ID3D11RasterizerState> mpRasterizer;
//		bool mTwoSided;
//	};
//}
