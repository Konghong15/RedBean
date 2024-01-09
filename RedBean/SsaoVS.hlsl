cbuffer cbPerFrame : register(b0)
{
	float4x4 gViewToTexSpace; // Proj*Texture
	float4   gOffsetVectors[14];
	float4   gFrustumCorners[4];
};

struct VertexIn
{
	float3 PosL            : POSITION;
	float3 ToFarPlaneIndex : NORMAL;
	float2 Tex             : TEXCOORD;
};

struct VertexOut
{
    float4 PosH       : SV_POSITION;
    float3 ToFarPlane : TEXCOORD0;
	float2 Tex        : TEXCOORD1;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	
	// 정규화된 값을 그대로 전달하고, 인덱스를 통해 먼 평면 정보를 같이 전달한다.
	vout.PosH = float4(vin.PosL, 1.0f);
	vout.ToFarPlane = gFrustumCorners[vin.ToFarPlaneIndex.x].xyz;
	vout.Tex = vin.Tex;
	
    return vout;
}
