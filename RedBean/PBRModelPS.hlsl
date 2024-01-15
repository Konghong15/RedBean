#include "LightHelper.hlsli"

struct VS_OUTPUT
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float2 UV : TEXCOORD0;
	float3 N : TEXCOORD1;
	float3 T : TEXCOORD2;
};

cbuffer cbFrame : register(b0)
{
	DirectionLight gDirLights[3];
	int gLightCount;
	float3 gEyePosW;

	float4 gFogColor;
	float gFogStart;
	float gFogRange;
	bool gUseFog;
	bool gUseIBL;
};

cbuffer cbMaterial : register(b1)
{
	bool gUseBaseColorMap;
	bool gUseNormal;
	bool gUseEmissiveMap;
	bool gUseAlpha;
	bool gUseMetalnessMap;
	bool gUseRoughnessMap;
}

Texture2D gAlbedoMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gEmissiveMap : register(t2);
Texture2D gOpacityMap : register(t3);
Texture2D gMetalnessMap : register(t4);
Texture2D gRoughnessMap : register(t5);

TextureCube gSpecularCubeMap : register(t6);
TextureCube gIrradianceCubeMap : register(t7);
Texture2D gSpecularBRDF_LUT : register(t8);

SamplerState gSamplerAnisotropic : register(s0); //	D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP
SamplerState gSP_BRDF_LUT_Sampler : register(s1); // D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP

static const float PI = 3.141592;
static const float Epsilon = 0.00001;
static const uint NumLights = 3;
static const float3 Fdielectric = 0.04;

float ndfGGX(float ndoth, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (ndoth * ndoth) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

float gaSchlickGGX(float ndotl, float ndotv, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	return gaSchlickG1(ndotl, k) * gaSchlickG1(ndotv, k);
}

float3 fresnelSchlick(float3 F0, float cosTheta)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

uint querySpecularTextureLevels()
{
	uint width, height, levels;
	gSpecularCubeMap.GetDimensions(0, width, height, levels);
	return levels;
}

float4 main(VS_OUTPUT Input) : SV_TARGET
{
	float3 toEye = gEyePosW - Input.PosW;
	float distanceEye = length(toEye);
	toEye /= distanceEye;

	float3 normal = normalize(Input.N);

	if (gUseNormal)
	{
		normal = gNormalMap.Sample(gSamplerAnisotropic, Input.UV).xyz;
		normal = NormalSampleToWorldSpace(normal, Input.N, Input.T);
	}

	float3 albedo = 1.f;

	if (gUseBaseColorMap)
	{
		albedo = gAlbedoMap.Sample(gSamplerAnisotropic, Input.UV).rgb;
	}

	float3 emissive = 0.f;

	if (gUseEmissiveMap)
	{
		emissive = gEmissiveMap.Sample(gSamplerAnisotropic, Input.UV).r;
	}

	float opacity = 1.f;

	if (gUseAlpha)
	{
		emissive = gOpacityMap.Sample(gSamplerAnisotropic, Input.UV).r;
	}

	float metalness = 0.f;

	if (gUseMetalnessMap)
	{
		metalness = gMetalnessMap.Sample(gSamplerAnisotropic, Input.UV).r;
	}

	float roughness = 0.f;

	if (gUseRoughnessMap)
	{
		roughness = gRoughnessMap.Sample(gSamplerAnisotropic, Input.UV).r;
	}

	float ndotv = max(0.f, dot(normal, toEye));
	float3 viewReflect = reflect(normal, -toEye);

	float3 F0 = lerp(Fdielectric, albedo, metalness);

	float3 directLighting = 0.0;

	[unroll]
	for (uint i = 0; i < gLightCount; ++i)
	{
		float3 lightInv = -gDirLights[i].Direction;
		// float3 lradiance = gDirLights[i].radiance; 이걸 무슨 값으로 반영해야할까

		float3 halfVec = normalize(lightInv + toEye);

		float ndotl = max(0.0, dot(normal, lightInv));
		float ndoth = max(0.0, dot(normal, halfVec));

		float3 F = fresnelSchlick(F0, max(0.0, dot(halfVec, toEye))); // 프레넬
		float D = ndfGGX(ndoth, max(Epsilon, roughness)); // 정반사 분포도
		float G = gaSchlickGGX(ndotl, ndotv, roughness); // 기하 감쇠율
		float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * ndotl * ndotv);

		float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);
		float3 diffuseBRDF = kd * albedo / PI;

		directLighting += (diffuseBRDF + specularBRDF) * ndotl; //* lradiance 
	}

	float3 ambientLighting = 0.f;

	if (gUseIBL)
	{
		float3 irradiance = gIrradianceCubeMap.Sample(gSamplerAnisotropic, normal).rgb; // 난반사 조도, 노말로 샘플링
		uint specularTextureLevels = querySpecularTextureLevels();
		float3 specularIrradiance = gSpecularCubeMap.SampleLevel(gSamplerAnisotropic, viewReflect, roughness * specularTextureLevels).rgb; // 정반사 조도, 뷰 반사벡터로 샘플링, 거칠기를 반영함?
		float2 specularBRDF = gSpecularBRDF_LUT.Sample(gSP_BRDF_LUT_Sampler, float2(ndotv, roughness)).rg; // 정반사 BRDF 룩업테이블, 노말 뷰의 cos값과 거칠기로 샘플링

		float3 F = fresnelSchlick(F0, ndotv); // 주변 조명에 대한 프레넬
		float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

		float3 kd = lerp(1.0 - F, 0.0, metalness); // 확산 기여 계수??
		float3 diffuseIBL = kd * albedo * irradiance;

		ambientLighting = diffuseIBL + specularIBL;
	}

	return float4(directLighting + ambientLighting + emissive, opacity);
}
