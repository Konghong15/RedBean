#pragma once

#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffer.h"

namespace Rgph
{
	class RenderGraph;
}

class PointLight
{
public:
	PointLight(Graphics& grapics, float radius = 0.5f);
	~PointLight() = default;

	void SpawnControlWindow();
	void Reset();
	void Submit() const;
	void Bind(Graphics& grapics, Matrix view) const;
	void LinkTechniques(Rgph::RenderGraph&);

private:
	struct PointLightCBuf
	{
		alignas(16) Vector3 pos;
		alignas(16) Vector3 ambient;
		alignas(16) Vector3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightCBuf cbData;
	mutable SolidSphere mesh;
	mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;
};