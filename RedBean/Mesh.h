#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct Node;

struct Bone
{
	string Name;
	size_t Index;
	Matrix OffsetMatrix;

	Node* NodeRef;
};

enum class eMeshType
{
	Static,
	Skinned
};

class Mesh
{
public:
	Mesh(aiMesh* mesh, eMeshType meshType);
	~Mesh() = default;

private:
	directXWrapper::VertexBuffer mVB;
	directXWrapper::IndexBuffer mIB;
	std::vector<Bone> Bones;
};
