#pragma once

#include "Graphics.h"
#include "ConstantBuffer.h"
#include "LightHelper.h"

namespace Rgph
{
	class RenderGraph;
}

class FrameLight
{
public:
	FrameLight(Graphics& graphics)
		: mCBuffer(graphics)
	{
		Reset();
	}
	~FrameLight() = default;

	// void SpawnControlWindow();
	void Reset()
	{
		// ÈÄ±¤
		mCBData.DirectionLights[0].Ambient = { 0.5f, 0.5f, 0.5f, 0.5f };
		mCBData.DirectionLights[0].Diffuse = { 1.f, 1.f,1.f, 1.f };
		mCBData.DirectionLights[0].Specular = { 1.f, 1.f, 1.f, 32.f };
		mCBData.DirectionLights[0].Direction = { 0.f, 0.f, 1.f };
		mCBData.DirectionLights[0].Direction.Normalize();

		// ¾ÕÁÂÃø
		mCBData.DirectionLights[1].Ambient = { 0.5f, 0.5f, 0.5f, 0.5f };
		mCBData.DirectionLights[1].Diffuse = { 0.5f, 0.5f, 0.5f, 0.5f };
		mCBData.DirectionLights[1].Specular = { 0.5f, 0.5f, 0.5f, 0.5f };
		mCBData.DirectionLights[1].Direction = { 1.f, 1.f, 1.f };
		mCBData.DirectionLights[1].Direction.Normalize();

		// ¾Õ¿ìÃø
		mCBData.DirectionLights[2].Ambient = { 0.5f, 0.5f, 0.5f, 0.5f };
		mCBData.DirectionLights[2].Diffuse = { 0.5f, 0.5f, 0.5f, 0.5f };
		mCBData.DirectionLights[2].Specular = { 0.5f, 0.5f, 0.5f, 0.5f };
		mCBData.DirectionLights[2].Direction = { -1.f , 1.f, 1.f };
		mCBData.DirectionLights[2].Direction.Normalize();

		mCBData.LightCount = 3;

		mCBData.EyePosW = { 0, };

		mCBData.FogColor = { 0,0,0,0 };

		mCBData.FogStart = 500.f;

		mCBData.FogRange = 1000.f;

		mCBData.bUseFog = false;
	}

	void Submit() const
	{
		// render data submit
	}

	void Bind(Graphics& graphics, Vector3 cameraPos)
	{
		mCBData.EyePosW = cameraPos;

		mCBuffer.Update(graphics, mCBData);
		mCBuffer.Bind(graphics);
	}

	void LinkTechnique(Rgph::RenderGraph& renderGraph)
	{
		// render data linking
	}

private:
	struct FrameLightCBuf
	{
		light::DirectionLight DirectionLights[3];
		int LightCount;
		Vector3 EyePosW;
		Vector4 FogColor;
		float FogStart;
		float FogRange;
		int bUseFog;
		int unused;
	};

private:
	FrameLightCBuf mCBData;
	mutable Bind::PixelConstantBuffer<FrameLightCBuf> mCBuffer;
};

