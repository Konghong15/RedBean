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

	// ������ �����̸� ������ �ʿ� �׸��� �ʴ´�.
	clip(diffuse.a - 0.15f);
}