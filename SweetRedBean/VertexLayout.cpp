#include "pch.h"

#include "VertexLayout.h"

namespace dynamic
{
	D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const
	{
		switch (mType)
		{
		case Position2D:
			return generateDesc<Position2D>(GetOffset());
		case Position3D:
			return generateDesc<Position3D>(GetOffset());
		case Texture2D:
			return generateDesc<Texture2D>(GetOffset());
		case Normal:
			return generateDesc<Normal>(GetOffset());
		case Tangent:
			return generateDesc<Tangent>(GetOffset());
		case Bitangent:
			return generateDesc<Bitangent>(GetOffset());
		case Float3Color:
			return generateDesc<Float3Color>(GetOffset());
		case Float4Color:
			return generateDesc<Float4Color>(GetOffset());
		case BGRAColor:
			return generateDesc<BGRAColor>(GetOffset());
		default:
			assert("Invalid element type" && false);
		}

		return { "INVALID",0,DXGI_FORMAT_UNKNOWN,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };
	}
	const char* VertexLayout::Element::GetCode() const
	{
		switch (mType)
		{
		case Position2D:
			return Map<Position2D>::code;
		case Position3D:
			return Map<Position3D>::code;
		case Texture2D:
			return Map<Texture2D>::code;
		case Normal:
			return Map<Normal>::code;
		case Tangent:
			return Map<Tangent>::code;
		case Bitangent:
			return Map<Bitangent>::code;
		case Float3Color:
			return Map<Float3Color>::code;
		case Float4Color:
			return Map<Float4Color>::code;
		case BGRAColor:
			return Map<BGRAColor>::code;
		default:
			assert("Invalid element type" && false);
		}

		return "Invalid";
	}
}