#pragma once

#include "SkinnedData.h"

class Material;
class MeshGeometry;
class SkinnedData;

class SkinnedModel
{
public:
	shared_ptr<Material> Material = nullptr;
	shared_ptr<MeshGeometry> MeshGeometry = nullptr;
	shared_ptr<SkinnedData> SkinnedData = nullptr;
};

struct SkinnedModelInstance
{
public:
	virtual void Render();
	
	inline void UpdateToRootTransforms(float deltaTime);

	shared_ptr<SkinnedModel> SkinnedModel;
	float TimePos;
	string ClipName;
	Matrix World;
	vector<Matrix> FinalTransforms;
};

void SkinnedModelInstance::UpdateToRootTransforms(float deltaTime)
{
	TimePos += deltaTime;
	SkinnedModel->SkinnedData->GetFinalTransforms(ClipName, TimePos, &FinalTransforms);

	// Loop animation
	if (TimePos > SkinnedModel->SkinnedData->GetClipEndTime(ClipName))
	{
		TimePos = 0.0f;
	}
}