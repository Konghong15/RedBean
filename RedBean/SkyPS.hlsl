
struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

TextureCube gCubeMap : register(t0); 
SamplerState gSamLinear : register(s0);

float4 main(VertexOut pin) : SV_Target
{
	// ��ȸ���ͷ� ���ø�
	float4 color = gCubeMap.Sample(gSamLinear, pin.PosL); 
	
	return color; 
}
