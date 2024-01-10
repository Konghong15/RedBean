struct VS_INPUT
{
	float3 position : POSITION;
	float2 UV : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 UV : TEXCOORD0;
	float3 viewDir : TEXCOORD1;
	float3 T : TEXCOORD2;
	float3 B : TEXCOORD3;
	float3 N : TEXCOORD4;
};

cbuffer cbObject : register(b0)
{
	matrix WVP;
	float4 camPosW;
};

VS_OUTPUT main(VS_INPUT Input)
{ 
	VS_OUTPUT Output;
	
	Output.position = mul(float4(Input.position, 1.f), WVP);
	Output.UV = Input.UV;

	float4 worldPosition = mul(Input.position, worldMat);
	float3 viewDir = worldPosition.xyz - camPosW.xyz;
	Output.viewDir = normalize(viewDir);

	float3 worldNormal = mul(Input.normal, (float3x3)worldMat);
	Output.N = normalize(worldNormal);
	
	float3 worldTangent = mul(Input.tangent, (float3x3)worldMat);
	Output.T = normalize(worldTangent);
	
	Output.B = cross(worldNormal, worldTangent);

	return Output;
}
