#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace resource
{
	class Mesh
	{
	public:
		Mesh(ID3D11Device* device, aiMesh* mesh);
		~Mesh() = default;

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
		MeshResource() = default;
		~MeshResource() = default;

		bool Init(ID3D11Device* device, string filename);

		const string& GetFileName() const { return mFileName; }
		const vector<Mesh>& GetMeshes() const { return mMeshes; }

	private:
		string mFileName;
		vector<Mesh> mMeshes;
	};

	struct Node;

	struct Bone
	{
		size_t Index;
		string Name;
		Matrix OffsetMatrix;
	};

	class SkinnedMesh
	{
	public:
		SkinnedMesh(ID3D11Device* device, aiMesh* mesh);
		~SkinnedMesh() = default;

		string mName;
		vector<builtIn::vertex::PosTexNormalTanSkinned> mVertices;
		vector<UINT> mIndices;
		directXWrapper::VertexBuffer mVB;
		directXWrapper::IndexBuffer mIB;
		vector<Bone> mBones;
	};

	class SkinnedMeshResource
	{
	public:
		SkinnedMeshResource() = default;
		~SkinnedMeshResource() = default;

		bool Init(ID3D11Device* device, string filename);

		const string& GetFileName() const { return mFileName; }
		const vector<SkinnedMesh>& GetSkinnedMeshes() const { return mSkinnedMeshes; }

	private:
		string mFileName;
		vector<SkinnedMesh> mSkinnedMeshes;
	};
}