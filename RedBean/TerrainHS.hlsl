#include "Terrain.hlsli"

float CalcTessFactor(float3 p)
{
	float d = distance(p, gEyePosW);
	float s = saturate((d - gMinDist) / (gMaxDist - gMinDist));

	return pow(2, (lerp(gMaxTess, gMinTess, s)));
}

bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
	float3 n = abs(plane.xyz);
	float r = dot(extents, n);
	float s = dot(float4(center, 1.0f), plane);

	return (s + r) < 0.0f;
}

bool AabbOutsideFrustumTest(float3 center, float3 extents, float4 frustumPlanes[6])
{
	for (int i = 0; i < 6; ++i)
	{
		if (AabbBehindPlaneTest(center, extents, frustumPlanes[i]))
		{
			return true;
		}
	}

	return false;
}

PatchTess ConstantHS(InputPatch<VertexOut, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	float minY = patch[0].BoundsY.x;
	float maxY = patch[0].BoundsY.y;

	float3 vMin = float3(patch[2].PosW.x, minY, patch[2].PosW.z);
	float3 vMax = float3(patch[1].PosW.x, maxY, patch[1].PosW.z);

	float3 boxCenter = 0.5f * (vMin + vMax);
	float3 boxExtents = 0.5f * (vMax - vMin);
	if (AabbOutsideFrustumTest(boxCenter, boxExtents, gWorldFrustumPlanes))
	{
		pt.EdgeTess[0] = 0.0f;
		pt.EdgeTess[1] = 0.0f;
		pt.EdgeTess[2] = 0.0f;
		pt.EdgeTess[3] = 0.0f;

		pt.InsideTess[0] = 0.0f;
		pt.InsideTess[1] = 0.0f;

		return pt;
	}
	else
	{
		float3 e0 = 0.5f * (patch[0].PosW + patch[2].PosW);
		float3 e1 = 0.5f * (patch[0].PosW + patch[1].PosW);
		float3 e2 = 0.5f * (patch[1].PosW + patch[3].PosW);
		float3 e3 = 0.5f * (patch[2].PosW + patch[3].PosW);
		float3  c = 0.25f * (patch[0].PosW + patch[1].PosW + patch[2].PosW + patch[3].PosW);

		pt.EdgeTess[0] = CalcTessFactor(e0);
		pt.EdgeTess[1] = CalcTessFactor(e1);
		pt.EdgeTess[2] = CalcTessFactor(e2);
		pt.EdgeTess[3] = CalcTessFactor(e3);

		pt.InsideTess[0] = CalcTessFactor(c);
		pt.InsideTess[1] = pt.InsideTess[0];

		return pt;
	}
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut main(InputPatch<VertexOut, 4> p,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
	HullOut hout;

	hout.PosW = p[i].PosW;
	hout.Tex = p[i].Tex;

	return hout;
}
