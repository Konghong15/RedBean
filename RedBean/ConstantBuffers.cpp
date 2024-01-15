#include "pch.h"

#include "ConstantBuffers.h"

namespace builtIn
{
	directXWrapper::ConstantBuffer<ConstantBuffers::TransformW> ConstantBuffers::TransformWCB;
	directXWrapper::ConstantBuffer<ConstantBuffers::TransformVP> ConstantBuffers::TransformVPCB;
	directXWrapper::ConstantBuffer<ConstantBuffers::BonePalette> ConstantBuffers::BonePaletteCB;
	directXWrapper::ConstantBuffer<ConstantBuffers::LegacyMaterial> ConstantBuffers::LegacyMaterialCB;
	directXWrapper::ConstantBuffer<ConstantBuffers::PBRMaterial> ConstantBuffers::PBRMaterialCB;
	directXWrapper::ConstantBuffer<ConstantBuffers::Frame> ConstantBuffers::FrameCB;

	void ConstantBuffers::InitAll(ID3D11Device* device)
	{
		TransformWCB.Init(device);
		TransformVPCB.Init(device);
		BonePaletteCB.Init(device);
		LegacyMaterialCB.Init(device);
		PBRMaterialCB.Init(device);
		FrameCB.Init(device);
	}

	void ConstantBuffers::DestroyAll()
	{
		TransformWCB.Destroy();
		TransformVPCB.Destroy();
		BonePaletteCB.Destroy();
		LegacyMaterialCB.Destroy();
		PBRMaterialCB.Destroy();
		FrameCB.Destroy();
	}
}