#include "pch.h"

#include "Node.h"
#include "Mesh.h"
#include "ModelProbe.h"

Node::Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const Matrix& transform_in)
	: mId(id)
	, mMeshPtrs(std::move(meshPtrs))
	, mName(name)
	, mTransform(transform_in)
	, mAppliedTransform(Matrix::Identity)
{
}

void Node::Submit(Matrix accumulatedTransform) const
{
	// 추가 적용된 트랜스폼 * 자기 트랜스폼 * 부모로부터 누적된 트랜스폼
	const Matrix finalTransform = mAppliedTransform * mTransform * accumulatedTransform;

	for (const auto pm : mMeshPtrs)
	{
		pm->Submit(finalTransform);
	}
	for (const auto& pc : mChildPtrs)
	{
		pc->Submit(finalTransform);
	}
}

void Node::Accept(ModelProbe& probe)
{
	if (probe.PushNode(*this))
	{
		for (auto& cp : mChildPtrs)
		{
			cp->Accept(probe);
		}
		probe.PopNode(*this);
	}
}

void Node::Accept(TechniqueProbe& probe)
{
	for (auto& meshPtr : mMeshPtrs)
	{
		meshPtr->Accept(probe);
	}
}

void Node::addChild(std::unique_ptr<Node> pChild)
{
	assert(pChild);

	mChildPtrs.push_back(std::move(pChild));
}

void Node::SetAppliedTransform(Matrix transform)
{
	mAppliedTransform = transform;
}

const Matrix& Node::GetAppliedTransform() const
{
	return mAppliedTransform;
}

int Node::GetId() const
{
	return mId;
}
bool Node::HasChildren() const
{
	return mChildPtrs.size() > 0;
}
const std::string& Node::GetName() const
{
	return mName;
}