#pragma once

namespace dynamic
{
	struct BGRAColor
	{
		unsigned char a;
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	class VertexLayout
	{
#pragma region ElementType
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Tangent,
			Bitangent,
			Float3Color,
			Float4Color,
			BGRAColor,
			Count,
		};

		// enum 값에 대응되는 컴파일 타임 형식 정보를 제공
		template<ElementType> struct Map;

		template<> struct Map<Position2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Position";
			static constexpr const char* code = "P2";
		};
		template<> struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
			static constexpr const char* code = "P3";
		};
		template<> struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Texcoord";
			static constexpr const char* code = "T2";
		};
		template<> struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
			static constexpr const char* code = "N";
		};
		template<> struct Map<Tangent>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Tangent";
			static constexpr const char* code = "Nt";
		};
		template<> struct Map<Bitangent>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Bitangent";
			static constexpr const char* code = "Nb";
		};
		template<> struct Map<Float3Color>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
			static constexpr const char* code = "C3";
		};
		template<> struct Map<Float4Color>
		{
			using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "Color";
			static constexpr const char* code = "C4";
		};
		template<> struct Map<BGRAColor>
		{
			using SysType = dynamic::BGRAColor;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "Color";
			static constexpr const char* code = "C8";
		};

		// ElementType의 형식 정보 클래스
		class Element
		{
		public:
			Element(ElementType type, size_t offset)
				: mType(type)
				, mOffset(offset)
			{
			}
			~Element() = default;

			static constexpr size_t SizeOf(ElementType type)
			{
				switch (type)
				{
				case Position2D:
					return sizeof(Map<Position2D>::SysType);
				case Position3D:
					return sizeof(Map<Position3D>::SysType);
				case Texture2D:
					return sizeof(Map<Texture2D>::SysType);
				case Normal:
					return sizeof(Map<Normal>::SysType);
				case Tangent:
					return sizeof(Map<Tangent>::SysType);
				case Bitangent:
					return sizeof(Map<Bitangent>::SysType);
				case Float3Color:
					return sizeof(Map<Float3Color>::SysType);
				case Float4Color:
					return sizeof(Map<Float4Color>::SysType);
				case BGRAColor:
					return sizeof(Map<BGRAColor>::SysType);
				default:
					assert("Invalid element type" && false);
				}

				return 0u;
			}
			D3D11_INPUT_ELEMENT_DESC GetDesc() const;
			const char* GetCode() const;

			ElementType GetType() const { return mType; }
			size_t GetSize() const { return SizeOf(mType); }
			size_t GetOffset() const { return mOffset; }
			size_t GetOffsetAfter() const { return mOffset + GetSize(); };

		private:
			template<ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC generateDesc(size_t offset);

		private:
			ElementType mType;
			size_t mOffset;
		};
#pragma endregion

	public:
		template<ElementType Type>
		inline const Element& Create() const;
		inline const Element& CreateByIndex(size_t i) const;

		inline VertexLayout& Append(ElementType type);

		inline size_t GetSize() const;
		inline size_t GetElementCount() const;
		inline vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const;
		inline string GetCode() const;

	private:
		std::vector<Element> mElements;
	};

#pragma region inlineFunc
	template<VertexLayout::ElementType type>
	constexpr D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::generateDesc(size_t offset)
	{
		return { Map<type>::semantic,0,Map<type>::dxgiFormat,0,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0 };
	}

	template<VertexLayout::ElementType Type>
	const VertexLayout::Element& VertexLayout::Create() const
	{
		for (auto& element : mElements)
		{
			if (element.GetType() == Type)
			{
				return element;
			}
		}

		assert("Could not resolve element type" && false);
		return mElements.front();
	}
	const VertexLayout::Element& VertexLayout::CreateByIndex(size_t i) const
	{
		return mElements[i];
	}

	VertexLayout& VertexLayout::Append(ElementType type)
	{
		mElements.push_back({ type, GetSize() });
		return *this;
	}

	size_t VertexLayout::GetSize() const
	{
		if (mElements.empty())
		{
			return 0u;
		}

		return mElements.back().GetOffsetAfter();
	}
	size_t VertexLayout::GetElementCount() const
	{
		return mElements.size();
	}
	vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const
	{
		vector<D3D11_INPUT_ELEMENT_DESC> desc;
		desc.reserve(GetElementCount());

		for (const auto& element : mElements)
		{
			desc.push_back(element.GetDesc());
		}

		return desc;
	}
	string VertexLayout::GetCode() const
	{
		string code;

		for (const auto& element : mElements)
		{
			code += element.GetCode();
		}

		return code;
	}
#pragma endregion
}