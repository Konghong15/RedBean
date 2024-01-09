#pragma once

#include "SkinnedData.h"

class Model;

class SkinnedModel
{
public:
	shared_ptr<Model> Model = nullptr;
	SkinnedData SkinnedData;
};

struct SkinnedModelInstance
{
public:
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
	SkinnedModel->SkinnedData.GetFinalTransforms(ClipName, TimePos, &FinalTransforms);

	// Loop animation
	if (TimePos > SkinnedModel->SkinnedData.GetClipEndTime(ClipName))
	{
		TimePos = 0.0f;
	}
}