#include "LightHelper.hlsli"

struct VS_OUTPUT
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float2 UV : TEXCOORD0;
	float3 N : TEXCOORD1;
	float3 T : TEXCOORD2;
};

// lightCB
cbuffer cbFrame : register(b0)
{
	DirectionLight gDirLights[3];
	int gLightCount;
	float3 gEyePosW;

	float4 gFogColor;
	float gFogStart;
	float gFogRange;
	bool gUseFog;
};

cbuffer cbMaterial : register(b1)
{
	bool gUseDiffuse;
	bool gUseNormal;
	bool gUseSpecular;
	bool gUseAlpha;
}

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecularMap : register(t2);
Texture2D gAlphaMap : register(t3);

SamplerState gSamplerLinear : register(s0);

float4 main(VS_OUTPUT Input) : SV_TARGET
{
	float3 toEye = gEyePosW - Input.PosW;
	float distanceEye = length(toEye);
	toEye /= distanceEye;

	float3 normal = normalize(Input.N);

	if (gUseNormal)
	{
		normal = gNormalMap.Sample(gSamplerLinear, Input.UV).xyz;
		normal = NormalSampleToWorldSpace(normal, Input.N, Input.T);
	}

	// 요 부분 어찌 다룰지 조금 더 생각해보자
	Material material;
	material.Specular = 0.f;
	material.Diffuse = 0.f;
	material.Ambient = 0.5f;

	if (gUseDiffuse)
	{
		material.Diffuse = gDiffuseMap.Sample(gSamplerLinear, Input.UV);
		// clip(material.Diffuse.a - 0.1f);
	}


	if (gUseSpecular)
	{
		material.Specular = gSpecularMap.Sample(gSamplerLinear, Input.UV);
	}

	float4 litColor = 1.f;

	if (gLightCount > 0)
	{
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		[unroll]
		for (int i = 0; i < 1; ++i)
		{
			float4 A, D, S;
			ComputeDirectionLight(material, gDirLights[i], normal, toEye, A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}

		litColor = ambient + diffuse + spec;
	}

	if (gUseFog)
	{
		float fogLerp = saturate((distanceEye - gFogStart) / gFogRange);
		litColor = lerp(litColor, gFogColor, fogLerp);
	}

	if (gUseAlpha)
	{
		litColor.a = gAlphaMap.Sample(gSamplerLinear, Input.UV).x;
	}

	return litColor;
}
