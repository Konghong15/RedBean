
struct VertexOut
{
	float4 PosH       : SV_POSITION;
    float3 PosV       : POSITION;
    float3 NormalV    : NORMAL;
	float2 Tex        : TEXCOORD0;
};

Texture2D gDiffuseMap : register(t0);
SamplerState samLinear : register(s0);

float4 main(VertexOut pin) : SV_Target
{
    pin.NormalV = normalize(pin.NormalV);
	float4 texColor = gDiffuseMap.Sample( samLinear, pin.Tex );
	clip(texColor.a - 0.1f);
	
	return float4(pin.NormalV, pin.PosV.z);
}