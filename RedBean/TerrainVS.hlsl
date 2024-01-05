#include "Terrain.hlsli"

VertexOut main(VertexIn vin)
{
	VertexOut vout;

	// ���̸��� ���ø��ؼ� �ݿ����ش�.
	vout.PosW = vin.PosL;
	vout.PosW.y = gHeightMap.SampleLevel(gSamHeightmap, vin.Tex, 0).r;
	vout.Tex = vin.Tex;
	vout.BoundsY = vin.BoundsY;

	return vout;
}