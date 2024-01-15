#pragma once

#include "ConstantBuffer.h"
#include "LightHelper.h"

namespace builtIn
{
	class ConstantBuffers
	{
	public:
		struct TransformW
		{
			Matrix World;
			Matrix WorldInvTranspose;
			Matrix Shadow;
		};

		struct TransformVP
		{
			Matrix View;
			Matrix Proj;
		};

		struct BonePalette
		{
			Matrix BoneMatrix[128];
		};

		struct LegacyMaterial
		{
			int bUseDiffuseMap = true;
			int bUseNormalMap = true;
			int bUseSpecularMap = true;
			int bUseOpacityMap = true;
		};

		struct PBRMaterial
		{
			int bUseBaseColorMap = true;
			int bUseNormalMap = true;
			int bUseEmissiveMap = true;
			int bUseOpacityMap = true;
			int bUseMetalnessMap = true;
			int bUseRoughnessMap = true;
			int unused[2];
		};

		struct Frame
		{
			common::DirectionLight DirLight[3];
			int LightCount;
			Vector3 EyePos;
			Vector4 FogColor;
			float FogStart;
			float FogRange;
			int bUseFog;
			int bUseIBL; // legacy에서는 사용 안함
		};

	public:
		static void InitAll(ID3D11Device* device);
		static void DestroyAll();

	public:
		static directXWrapper::ConstantBuffer<TransformW> TransformWCB;
		static directXWrapper::ConstantBuffer<TransformVP> TransformVPCB;
		static directXWrapper::ConstantBuffer<BonePalette> BonePaletteCB;
		static directXWrapper::ConstantBuffer<LegacyMaterial> LegacyMaterialCB;
		static directXWrapper::ConstantBuffer<PBRMaterial> PBRMaterialCB;
		static directXWrapper::ConstantBuffer<Frame> FrameCB;
	};
}
