#pragma once


namespace directXWrapper
{
	class VertexBuffer;
	class IndexBuffer;
}

// 메시를 저장하는 기하구조
class MeshGeometry
{
#pragma region struct
public:
	struct Subset
	{
		UINT Id;
		string Name;
		UINT VertexStart = 0;
		UINT VertexCount = 0;
		UINT FaceStart = 0;
		UINT FaceCount = 0;
	};
#pragma endregion

public:
	MeshGeometry(aiScene* scene);
	~MeshGeometry() = default;

	void Draw(ID3D11DeviceContext* context, size_t index);
 	shared_ptr<directXWrapper::VertexBuffer> VB = nullptr;
	shared_ptr<directXWrapper::IndexBuffer> IB = nullptr;
	vector<Subset> Subsets;
};

