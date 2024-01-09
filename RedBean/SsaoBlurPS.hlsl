
cbuffer cbPerFrame
{
	float gTexelWidth;
	float gTexelHeight;
	bool gHorizontalBlur;
};

Texture2D gNormalDepthMap : register(t0);
Texture2D gInputImage : register(t1);
SamplerState gSamPoint : register(s0);

struct VertexOut
{
    float4 PosH  : SV_POSITION;
	float2 Tex   : TEXCOORD;
};

static float gWeights[11] = 
{
	0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
};

static int gBlurRadius = 5;

float4 main(VertexOut pin) : SV_Target
{
	float2 texOffset;

	if(gHorizontalBlur)
	{
		texOffset = float2(gTexelWidth, 0.0f);
	}
	else
	{
		texOffset = float2(0.0f, gTexelHeight);
	}
	
	// 중앙 값은 항상 총합에 기여하도록 미리 처리한다.
	float4 color      = gWeights[5] * gInputImage.SampleLevel(gSamPoint, pin.Tex, 0.0);
	float totalWeight = gWeights[5];
	 
	// 법선/깊이 데이터 샘플링
	float4 centerNormalDepth = gNormalDepthMap.SampleLevel(gSamPoint, pin.Tex, 0.0f);

	for(float i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		// 중앙값은 다시 처리하지 않는다.
		if( i == 0 )
			continue;

		float2 tex = pin.Tex + i * texOffset;
		float4 neighborNormalDepth = gNormalDepthMap.SampleLevel(gSamPoint, tex, 0.0f);

		// 중앙과 이웃의 법선이 너무 차이나거나 깊이 차이가 너무 크면 해당 표본은 흐리기에서 제외한다.
		if( dot(neighborNormalDepth.xyz, centerNormalDepth.xyz) < 0.8f &&
		    abs(neighborNormalDepth.a - centerNormalDepth.a) > 0.2f )
		{
			continue;
		}

		float weight = gWeights[i + gBlurRadius];

		color += weight * gInputImage.SampleLevel(gSamPoint, tex, 0.0);
		totalWeight += weight;
	}

	// 적용된 가중치들과 합을 나눠준다.
	return color / totalWeight;
}
