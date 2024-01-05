
struct VertexIn
{
	float3 pos : POSITION;
};

float4 main(VertexIn vin) : SV_POSITION
{
	return float4(vin.pos, 1);
}