#pragma once

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
	};

	class NodeResource
	{
	public:
		NodeResource() = default;
		~NodeResource() = default;

		bool Init(string filename);

		const string& GetFileName() { return mFilename; }
		const Node* GetRootNode() { return mRoot; }
		const vector<Node*> GetNodes() { return mNodes; }

	private:
		enum { RESERVE_SIZE = 256 };

		string mFilename;
		Node* mRoot;
		vector<Node*> mNodes;
	};
}