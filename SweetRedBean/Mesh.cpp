#include "pch.h"

#include "Mesh.h"
#include "Surface.h"
#include "DynamicConstant.h"
#include "LayoutCodex.h"
#include "Stencil.h"

// Mesh�� ������ IDrawable���� �ñ��.
Mesh::Mesh(Graphics& grapics, const Material& mat, const aiMesh& mesh, float scale)
	: IDrawable(grapics, mat, mesh, scale)
{
}

void Mesh::Submit(Matrix accumulatedTranform) const
{
	mTransform = accumulatedTranform;

	IDrawable::Submit();
}

Matrix Mesh::GetTransform() const
{
	return mTransform;
}
