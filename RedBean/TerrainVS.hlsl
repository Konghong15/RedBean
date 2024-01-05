#include "Terrain.hlsli"

VertexOut main(VertexIn vin)
{
	VertexOut vout;

	// 높이맵을 샘플링해서 반영해준다.
	vout.PosW = vin.PosL;
	vout.PosW.y = gHeightMap.SampleLevel(gSamHeightmap, vin.Tex, 0).r;
	vout.Tex = vin.Tex;
	vout.BoundsY = vin.BoundsY;

	return vout;
}