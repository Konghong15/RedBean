#include "Terrain.hlsli"

[domain("quad")]
DomainOut main(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad)
{
	DomainOut dout;

	// 겹선형 보간으로 정점 데이터 생성
	dout.PosW = lerp(lerp(quad[0].PosW, quad[1].PosW, uv.x),
					lerp(quad[2].PosW, quad[3].PosW, uv.x),
					uv.y);
	dout.Tex = lerp(lerp(quad[0].Tex, quad[1].Tex, uv.x),
					lerp(quad[2].Tex, quad[3].Tex, uv.x),
					uv.y);

	// 샘플링 비율, wrap이니 높으면 타일링이 촘촘해진다.
	dout.TiledTex = dout.Tex * gTexScale;

	// 최상위 밉맵 수준으로 높이맵을 샘플링한다.
	dout.PosW.y = gHeightMap.SampleLevel(gSamHeightmap, dout.Tex, 0).r;

	// 투영 변환
	dout.PosH = mul(float4(dout.PosW, 1.0f), gViewProj);

	return dout;
}
