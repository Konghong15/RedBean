#pragma once

struct Node
{
	std::string Name;
	size_t Index;
	size_t ParentIndex;

	Matrix ToParentMatrix = Matrix::Identity;
	Matrix ToRootMatrix = Matrix::Identity;

	Node* Parent;
	vector<Node*> Children;

	vector<pair<Mesh*, Material*>> MeshMaterial;
};

class Mesh;
class Material;
class AnimationClip;

class Model
{
public:
	Model(const aiScene* aiScene);
	~Model() = default;

	void UpdateBoneNodeIndex();

private:
	Node* mRoot;
	vector<Node*> mNodes;
	std::map<string, AnimationClip*> mAnimationClips;
};

