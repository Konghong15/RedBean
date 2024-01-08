#include "pch.h"

#include "MeshGeometry.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

void MeshGeometry::Draw(ID3D11DeviceContext* context, size_t index)
{
	assert(index < Subsets.size());

	VB->Bind(context);
	IB->Bind(context);

	context->DrawIndexed(Subsets[index].FaceCount * 3, Subsets[index].FaceStart * 3, 0);
}