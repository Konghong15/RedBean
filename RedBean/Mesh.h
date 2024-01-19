#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "VertexTypes.h"

namespace resource
{
	struct Node
	{
		enum { INVALID_INDEX = -1 };

		std::string Name;
		size_t Index;
		size_t ParentIndex;

		Matrix ToParentMatrix = Matrix::Identity;
		Matrix ToRootMatrix = Matrix::Identity;

		Node* Parent;
		vector<Node*> Children;

		vector<size_t> ContainMeshesIndex;
	};

	class Mesh
	{
	public:
		Mesh(ID3D11Device* device, aiMesh* mesh);
		~Mesh() = default;

		const string& GetName() const { return mName; }
		const vector<builtIn::vertex::PosTexNormalTan>& GetVertices() const { return mVertices; }
		const vector<UINT>& GetIndices() const { return mIndices; }
		const directXWrapper::VertexBuffer& GetVB() const { return mVB; }
		const directXWrapper::IndexBuffer& GetIB() const { return mIB; }

	private:
		string mName;
		vector<builtIn::vertex::PosTexNormalTan> mVertices;
		vector<UINT> mIndices;
		directXWrapper::VertexBuffer mVB;
		directXWrapper::IndexBuffer mIB;
	};

	class MeshResource
	{
	public:
		MeshResource(ID3D11Device* device, const aiScene* scene);
		~MeshResource();

		const string& GetName() const { return mName; }
		const vector<Mesh>& GetMeshes() const { return mMeshes; }
		const vector<Node*> GetNodes() const { return mNodes; }

	private:
		string mName;
		vector<Mesh> mMeshes;
		vector<Node*> mNodes;
	};

	struct Bone
	{
		string Name;
		size_t NodeIndedx;
		Matrix OffsetMatrix;
	};

	class SkinnedMesh
	{
	public:
		SkinnedMesh(ID3D11Device* device, aiMesh* mesh);
		~SkinnedMesh() = default;

		string mName;
		vector<builtIn::vertex::PosTexNormalTanSkinned> mVertices;
		vector<unsigned int> mIndices;
		directXWrapper::VertexBuffer mVB;
		directXWrapper::IndexBuffer mIB;
		vector<Bone> mBones;
	};

	class SkinnedMeshResource
	{
	public:
		SkinnedMeshResource(ID3D11Device* device, const aiScene* scene);
		~SkinnedMeshResource();

		vector<Node*>& GetNodes() { return mNodes; }

		const string& GetFileName() const { return mName; }
		const vector<SkinnedMesh>& GetSkinnedMeshes() const { return mSkinnedMeshes; }
		const vector<Node*>& GetNodes() const { return mNodes; }

	private:
		string mName;
		vector<SkinnedMesh> mSkinnedMeshes;
		vector<Node*> mNodes;
	};
}