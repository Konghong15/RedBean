//#pragma once
//
//#include "IBindable.h"
//
//class Graphics;
//
//class Step
//{
//public:
//	Step(size_t targetPass_in);
//	~Step() = default;
//
//	void Submit(class FrameCommander& frame, const class Drawable& drawable) const;
//	void Bind(Graphics& graphics) const;
//	void InitReferences(const class Drawable& parent);
//
//	void AddBindable(std::shared_ptr<bind::IBindable> bind_in);
//
//private:
//	size_t targetPass;
//	std::vector<std::shared_ptr<bind::IBindable>> bindables;
//};