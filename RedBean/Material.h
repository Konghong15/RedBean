#pragma once

namespace directXWraper
{
	class Texture;
	class ShaderProgram;
}

class Material
{
#pragma region struct
public:
	enum class eTexutreType
	{
		Diffuse,
		Specular,

	};

	struct Subset
	{
		string Name;
		map<eTexutreType, shared_ptr<directXWraper::Texture>> SRV;
	};
#pragma endregion

public:
	vector<Subset> Subsets;
};

