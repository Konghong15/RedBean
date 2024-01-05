#include "Terrain.hlsli"

[domain("quad")]
DomainOut main(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad)
{
	DomainOut dout;

	// �㼱�� �������� ���� ������ ����
	dout.PosW = lerp(lerp(quad[0].PosW, quad[1].PosW, uv.x),
					lerp(quad[2].PosW, quad[3].PosW, uv.x),
					uv.y);
	dout.Tex = lerp(lerp(quad[0].Tex, quad[1].Tex, uv.x),
					lerp(quad[2].Tex, quad[3].Tex, uv.x),
					uv.y);

	// ���ø� ����, wrap�̴� ������ Ÿ�ϸ��� ����������.
	dout.TiledTex = dout.Tex * gTexScale;

	// �ֻ��� �Ӹ� �������� ���̸��� ���ø��Ѵ�.
	dout.PosW.y = gHeightMap.SampleLevel(gSamHeightmap, dout.Tex, 0).r;

	// ���� ��ȯ
	dout.PosH = mul(float4(dout.PosW, 1.0f), gViewProj);

	return dout;
}
