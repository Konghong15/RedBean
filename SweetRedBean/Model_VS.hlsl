struct VS_INPUT
{
	float3 PosL : Position;
	float2 UV : Texcoord;
	float3 Normal : Normal;
	float3 Tangent : Tangent;
};

struct VS_OUTPUT
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float2 UV : TEXCOORD0;
	float3 N : TEXCOORD1;
	float3 T : TEXCOORD2;
};

cbuffer cbTransformW : register(b0)
{
	matrix gWorld;
	matrix gWVP;
};

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.PosW = mul(float4(Input.PosL, 1.f), gWorld).xyz;
	Output.PosH = mul(float4(Input.PosL, 1.f), gWVP);

	Output.UV = Input.UV;

	float3 worldNormal = mul(Input.Normal, (float3x3)gWorld);
	Output.N = normalize(worldNormal);

	float3 worldTangent = mul(Input.Tangent, (float3x3)gWorld);
	Output.T = normalize(worldTangent);

	return Output;
}
