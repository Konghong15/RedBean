//#pragma once
//
//#include "Step.h"
//
//class IDrawable;
//
//class Technique
//{
//public:
//	Technique() = default;
//	~Technique() = default;
//
//	void Submit(class FrameCommander& frame, const IDrawable& drawable) const;
//	void InitReferences(const IDrawable& parent);
//
//	void AddStep(Step step);
//
//	void SetIsActive(bool bIsActive);
//
//	bool IsActive() const;
//
//private:
//	bool mbIsActive = true;
//	std::vector<Step> steps;
//};