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
		Vector3 AttenuationParam; // 감쇠 매개변수 a0, a1, a2
		float pad;
	};

	struct SpotLight
	{
		Vector4 Ambient;
		Vector4 Diffuse;
		Vector4 Specular;
		Vector3 Direction;
		float Spot; // 원뿔에 사용될 지수
		Vector3 Position;
		float Range;
		Vector3 AttenuationParam; // 감쇠 매개변수 a0, a1, a2
		float pad;
	};

	struct Material
	{
		Vector4 Ambient;
		Vector4 Diffuse;
		Vector4 Specular; // specular의 마지막 성분을 지수로 사용함
		Vector4 Reflect;
	};
}