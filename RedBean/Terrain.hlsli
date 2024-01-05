#include "LightHelper.hlsli"

// struct

struct VertexIn
{
	float3 PosL     : POSITION;
	float2 Tex      : TEXCOORD0;
	float2 BoundsY  : TEXCOORD1;
};

struct VertexOut
{
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
	float2 BoundsY  : TEXCOORD1;
};

struct PatchTess
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

struct HullOut
{
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
};

struct DomainOut
{
	float4 PosH     : SV_POSITION;
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
	float2 TiledTex : TEXCOORD1;
};

// cbuffer

cbuffer cbPerObject : register(b0)
{
	float4x4 gViewProj;
	Material gMaterial;
};

cbuffer cbPerFrame : register(b1)
{
	DirectionLight gDirLights[3];
	float3 gEyePosW;
	float  gFogStart;
	float4 gFogColor;
	float  gFogRange;
	float gMinDist;
	float gMaxDist;
	float gMinTess;
	float gMaxTess;
	float gTexelCellSpaceU;
	float gTexelCellSpaceV;
	float gWorldCellSpace;
	float4 gWorldFrustumPlanes[6];
	float2 gTexScale;// = 50.0f;
};

// texture, sampler

Texture2DArray gLayerMapArray : register(t0);
Texture2D gBlendMap : register(t1);
Texture2D gHeightMap : register(t2);
SamplerState gSamHeightmap : register(s0);
SamplerState gSamLinear : register(s1);
