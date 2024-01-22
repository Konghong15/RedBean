#pragma once
#include "Graphics.h"
#include "IDrawable.h"

class Material;
class FrameCommander;
struct aiMesh;

class Mesh : public IDrawable
{
public:
	Mesh(Graphics& grapics, const Material& mat, const aiMesh& mesh, float scale = 1.0f);
	virtual ~Mesh() = default;

	void Submit(Matrix accumulatedTranform) const;

	Matrix GetTransform() const  override;

private:
	mutable Matrix mTransform;
};