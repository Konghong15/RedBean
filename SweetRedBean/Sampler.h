//#pragma once
//
//#include "IBindable.h"
//
//namespace bind
//{
//	class Sampler : public IBindable
//	{
//	public:
//		Sampler(Graphics& graphics);
//		~Sampler() = default;
//
//		static shared_ptr<Sampler> Resolve(Graphics& graphics);
//		static string GenerateUID();
//
//		virtual void Bind(Graphics& graphics)  override;
//
//		string GetUID() const  override;
//
//	protected:
//		ComPtr<ID3D11SamplerState> pSampler;
//	};
//}
