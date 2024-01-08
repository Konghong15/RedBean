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
	enum class TexutreType
	{
		Diffuse,
		Specular,

	};

	struct Subset
	{
		string Name;
		map<TexutreType, shared_ptr<directXWraper::Texture>> SRV;
	};
#pragma endregion

public:
	vector<Subset> Subsets;
};

