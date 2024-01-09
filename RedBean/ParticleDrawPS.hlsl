
struct GeoOut
{
	float4 PosH  : SV_Position;
	float4 Color : COLOR;
	float2 Tex   : TEXCOORD;
};

Texture2DArray gTexArray : register(t0);
SamplerState samLinear : register(s0);

float4 main(GeoOut pin) : SV_TARGET
{
	return gTexArray.Sample(samLinear, float3(pin.Tex, 0)) * pin.Color;
}