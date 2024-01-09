Texture2D gDiffuseMap;
SamplerState samLinear;

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex  : TEXCOORD;
};

void main(VertexOut pin)
{
	float4 diffuse = gDiffuseMap.Sample(samLinear, pin.Tex);

	// 투명한 재질이면 쉐도우 맵에 그리지 않는다.
	clip(diffuse.a - 0.15f);
}