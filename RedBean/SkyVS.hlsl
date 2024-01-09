cbuffer cbPerFrame : register(b0)
{
	float4x4 gWorldViewProj; // ī�޶� ��ġ�� ��ȯ ���
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

	// ��� ������ ī�޶� ��ġ���� ���ؼ��� �޽� ������ ���´�.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww; 

	// ��� ������ �ڽ��� ���ð����� ��ȸ ���ͷ� ����Ͽ� ť����� ���ø��Ѵ�.
	vout.PosL = vin.PosL; 

	return vout;
}
