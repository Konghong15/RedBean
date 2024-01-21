#pragma once

#include "Graphics.h"

class Model;
class Mesh;
class TechniqueProbe;
class ModelProbe;

class Node
{
	friend Model;

public:
	Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const Matrix& transform);

	void Submit(Matrix accumulatedTransform) const;
	void Accept(ModelProbe& probe);
	void Accept(TechniqueProbe& probe);

	void SetAppliedTransform(Matrix transform);

	const Matrix& GetAppliedTransform() const;
	int GetId() const;
	bool HasChildren() const;
	const std::string& GetName() const;

private:
	void addChild(std::unique_ptr<Node> pChild);

private:
	std::string mName;
	int mId;
	std::vector<std::unique_ptr<Node>> mChildPtrs;
	std::vector<Mesh*> mMeshPtrs;

	Matrix mTransform;
	Matrix mAppliedTransform;
};