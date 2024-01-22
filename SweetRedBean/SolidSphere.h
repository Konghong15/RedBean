#pragma once

#include "IDrawable.h"

class SolidSphere : public IDrawable
{
public:
	SolidSphere(Graphics& grapics, float radius);
	virtual ~SolidSphere() = default;

	void SetPos(Vector3 pos);
	Matrix GetTransform() const override;

private:
	Vector3 pos = { 1.0f,1.0f,1.0f };
};