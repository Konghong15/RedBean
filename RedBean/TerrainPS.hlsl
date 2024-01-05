#include "Terrain.hlsli"

float4 main(DomainOut pin) : SV_Target
{
	// ���� �ȼ��� ������ uv���� �����´�. 
	float2 leftTex = pin.Tex + float2(-gTexelCellSpaceU, 0.0f);
	float2 rightTex = pin.Tex + float2(gTexelCellSpaceU, 0.0f);
	float2 bottomTex = pin.Tex + float2(0.0f, gTexelCellSpaceV);
	float2 topTex = pin.Tex + float2(0.0f, -gTexelCellSpaceV);

	// ���� �ȼ����� ������ ���̰� 4���� �о�´�.
	float leftY = gHeightMap.SampleLevel(gSamHeightmap, leftTex, 0).r;
	float rightY = gHeightMap.SampleLevel(gSamHeightmap, rightTex, 0).r;
	float bottomY = gHeightMap.SampleLevel(gSamHeightmap, bottomTex, 0).r;
	float topY = gHeightMap.SampleLevel(gSamHeightmap, topTex, 0).r;

	// ���� �������� ��ĭ�� ������ �������� �߽� ���й��� �̿��� ���� ���͸� ���Ѵ�.
	float3 tangent = normalize(float3(2.0f * gWorldCellSpace, rightY - leftY, 0.0f));
	float3 bitan = normalize(float3(0.0f, bottomY - topY, -2.0f * gWorldCellSpace));
	float3 normalW = cross(tangent, bitan);

	float3 toEye = gEyePosW - pin.PosW;

	float distToEye = length(toEye);

	toEye /= distToEye;

	// ��Ƽ �ؽ�ó�� �ε��ؼ� ����ʿ� ����ġ�� ���� ��������
	float4 c0 = gLayerMapArray.Sample(gSamLinear, float3(pin.TiledTex, 0.0f));
	float4 c1 = gLayerMapArray.Sample(gSamLinear, float3(pin.TiledTex, 1.0f));
	float4 c2 = gLayerMapArray.Sample(gSamLinear, float3(pin.TiledTex, 2.0f));
	float4 c3 = gLayerMapArray.Sample(gSamLinear, float3(pin.TiledTex, 3.0f));
	float4 c4 = gLayerMapArray.Sample(gSamLinear, float3(pin.TiledTex, 4.0f));

	// ������� ���� ��ü�� ���� �뷫���� ������ ���̹Ƿ� Ȯ����� ���� uv���� ����Ѵ�.
	float4 t = gBlendMap.Sample(gSamLinear, pin.Tex);

	float4 texColor = c0;
	texColor = lerp(texColor, c1, t.r);
	texColor = lerp(texColor, c2, t.g);
	texColor = lerp(texColor, c3, t.b);
	texColor = lerp(texColor, c4, t.a);

	// ����ó�� + fog ó��

	float4 litColor = texColor;
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	[unroll]
	for (int i = 0; i < 1; ++i)
	{
		float4 A, D, S;

		ComputeDirectionLight(gMaterial,
			gDirLights[i],
			normalW, 
			toEye,
			A, D, S);

		ambient += A;
		diffuse += D;
		spec += S;
	}

	litColor = texColor * (ambient + diffuse) + spec;
	// litColor = (ambient + diffuse) + spec;

	float fogLerp = saturate((distToEye - gFogStart) / gFogRange);

	litColor = lerp(litColor, gFogColor, fogLerp);

	return litColor;
}