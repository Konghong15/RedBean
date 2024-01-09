cbuffer cbPerFrame : register(b0)
{
	float4x4 gWorldViewProj; // 카메라 위치로 변환 행렬
};

struct VertexIn
{
	float3 PosL : POSITION;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;

	// 모든 정점은 카메라 위치에서 재해석된 메시 구조를 갖는다.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww; 

	// 모든 정점은 자신의 로컬공간을 조회 벡터로 사용하여 큐브맵을 샘플링한다.
	vout.PosL = vin.PosL; 

	return vout;
}
