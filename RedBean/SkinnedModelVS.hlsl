struct VS_INPUT
{
	float3 position : POSITION;
	float2 UV : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	int4 Indices : INDICES;
	float4 Weights : WEIGHTS;
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

cbuffer cbBonePalette : register(b1)
{
	matrix boneMat[128];
}

VS_OUTPUT main(VS_INPUT Input)
{ 
	VS_OUTPUT Output;
	
	matrix combindedMatrix = mul(Input.Weights.x, boneMat[Input.Indices.x]);;
	for (int i = 1; i < 4; ++i)
	{
		if (Input.Indices[i] == -1)
		{
			break;
		}
	
		combindedMatrix += mul(Input.Weights[i], boneMat[Input.Indices[i]]);
	}

	Output.position = mul(float4(Input.position, 1.f), combindedMatrix); // 로컬 공간에 정의된 정점
	Output.position = mul(Output.position, WVP);
	Output.UV = Input.UV;

	float4 worldPosition = mul(Input.position, combindedMatrix);
	float3 viewDir = worldPosition.xyz - camPosW.xyz;
	Output.viewDir = normalize(viewDir);

	float3 worldNormal = mul(Input.normal, (float3x3)combindedMatrix);
	Output.N = normalize(worldNormal);
	
	float3 worldTangent = mul(Input.tangent, (float3x3)combindedMatrix);
	Output.T = normalize(worldTangent);
	Output.B = cross(worldNormal, worldTangent);

	return Output;
}
