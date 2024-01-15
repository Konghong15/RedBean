struct VS_INPUT
{
	float3 PosL : POSITION;
	float2 UV : UV;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	int4 Indices : INDICES;
	float4 Weights : WEIGHTS;
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

cbuffer cbBonePalette : register(b2)
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

	combindedMatrix = mul(combindedMatrix, gWorld);
	Output.PosH = mul(float4(Input.PosL, 1.f), combindedMatrix);
	Output.PosW = Output.PosH.xyz;

	Output.PosH = mul(Output.PosH, gView);
	Output.PosH = mul(Output.PosH, gProj);

	Output.UV = Input.UV;

	float3 worldNormal = mul(Input.Normal, (float3x3)combindedMatrix);
	Output.N = normalize(worldNormal);

	float3 worldTangent = mul(Input.Tangent, (float3x3)combindedMatrix);
	Output.T = normalize(worldTangent);

	return Output;
}
