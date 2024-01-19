struct VS_INPUT
{
	float3 PosL : POSITION;
	float2 UV : UV;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
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
	matrix gWorldInvTranspose;
	matrix gShadow;
};

cbuffer cbTransformVP : register(b1)
{
	matrix gView;
	matrix gProj;
}

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.PosH = mul(float4(Input.PosL, 1.f), gWorld);
	Output.PosW = Output.PosH.xyz;

	Output.PosH = mul(Output.PosH, gView);
	Output.PosH = mul(Output.PosH, gProj);

	Output.UV = Input.UV;

	float3 worldNormal = mul(Input.Normal, (float3x3)gWorld);
	Output.N = normalize(worldNormal);

	float3 worldTangent = mul(Input.Tangent, (float3x3)gWorld);
	Output.T = normalize(worldTangent);

	return Output;
}
