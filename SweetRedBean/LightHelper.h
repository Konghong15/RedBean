#pragma once

namespace light
{
	struct DirectionLight
	{
		Vector4 Ambient;
		Vector4 Diffuse;
		Vector4 Specular;
		Vector3 Direction;
		float pad;
	};

	struct PointLight
	{
		Vector4 Ambient;
		Vector4 Diffuse;
		Vector4 Specular;
		Vector3 Position;
		float Range;
		Vector3 AttenuationParam; // ���� �Ű����� a0, a1, a2
		float pad;
	};

	struct SpotLight
	{
		Vector4 Ambient;
		Vector4 Diffuse;
		Vector4 Specular;
		Vector3 Direction;
		float Spot; // ���Կ� ���� ����
		Vector3 Position;
		float Range;
		Vector3 AttenuationParam; // ���� �Ű����� a0, a1, a2
		float pad;
	};

	struct Material
	{
		Vector4 Ambient;
		Vector4 Diffuse;
		Vector4 Specular; // specular�� ������ ������ ������ �����
		Vector4 Reflect;
	};
}