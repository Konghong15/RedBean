#pragma once

#include "Graphics.h"
#include "VertexLayout.h"

namespace dynamic
{
	class Vertex
	{
		friend class VertexBuffer;

	public:
		template<VertexLayout::ElementType Type>
		auto& GetAttribute();
		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val);

		// 그냥 인스턴스 생성 못하도록 한 건가
	protected:
		inline Vertex(char* pData, const VertexLayout& layout);

	private:
		template<typename First, typename ...Rest>
		void setAttributeByIndexRecursive(size_t i, First&& first, Rest&&... rest);
		template<VertexLayout::ElementType DestLayoutType, typename SrcType>
		void setAttribute(char* pAttribute, SrcType&& val);

	private:
		char* mpData = nullptr;
		const VertexLayout& mLayout;
	};

#pragma region VertexInlineFunc
	template<VertexLayout::ElementType Type>
	auto& Vertex::GetAttribute()
	{
		auto pAttribute = mpData + mLayout.Create<Type>().GetOffset();
		return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
	}

	template<typename T>
	void Vertex::SetAttributeByIndex(size_t i, T&& val)
	{
		const auto& element = mLayout.CreateByIndex(i);
		auto pAttribute = mpData + element.GetOffset();

		switch (element.GetType())
		{
		case VertexLayout::Position2D:
			setAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Position3D:
			setAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Texture2D:
			setAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Normal:
			setAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Tangent:
			setAttribute<VertexLayout::Tangent>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Bitangent:
			setAttribute<VertexLayout::Bitangent>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Float3Color:
			setAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Float4Color:
			setAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::BGRAColor:
			setAttribute<VertexLayout::BGRAColor>(pAttribute, std::forward<T>(val));
			break;
		default:
			assert("Bad element type" && false);
		}
	}

	Vertex::Vertex(char* pData, const VertexLayout& layout)
		: mpData(pData)
		, mLayout(layout)
	{
		assert(pData != nullptr);
	}

	template<typename First, typename ...Rest>
	void Vertex::setAttributeByIndexRecursive(size_t i, First&& first, Rest&&... rest)
	{
		SetAttributeByIndex(i, std::forward<First>(first));
		setAttributeByIndexRecursive(i + 1, std::forward<Rest>(rest)...);
	}

	template<VertexLayout::ElementType DestLayoutType, typename SrcType>
	void Vertex::setAttribute(char* pAttribute, SrcType&& val)
	{
		using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;

		if constexpr (std::is_assignable<Dest, SrcType>::value)
		{
			*reinterpret_cast<Dest*>(pAttribute) = val;
		}
		else
		{
			assert("Parameter attribute type mismatch" && false);
		}
	}
#pragma endregion

	class ConstVertex
	{
	public:
		inline ConstVertex(const Vertex& vertex);
		~ConstVertex() = default;

		template<VertexLayout::ElementType Type>
		const auto& GetAttribute() const;

	private:
		Vertex mVertex;
	};

#pragma region ConstVertexInlineFunc
	ConstVertex::ConstVertex(const Vertex& vertex)
		: mVertex(vertex)
	{

	}

	template<VertexLayout::ElementType Type>
	const auto& ConstVertex::GetAttribute() const
	{
		return const_cast<Vertex&>(mVertex).GetAttribute<Type>();
	}
#pragma endregion

	class VertexBuffer
	{
	public:
		inline VertexBuffer(VertexLayout layout, size_t size = 0u);
		~VertexBuffer() = default;

		template<typename ...Params>
		void EmplaceBack(Params&&... params);
		inline void Resize(size_t newSize);

		inline Vertex Back();
		inline Vertex Front();
		inline Vertex operator[](size_t i);
		inline ConstVertex Back() const;
		inline ConstVertex Front() const;
		inline ConstVertex operator[](size_t i) const;

		inline const char* GetData() const;
		inline const VertexLayout& GetLayout() const;
		inline size_t GetSize() const;
		inline size_t GetSizeBytes() const;

	private:
		std::vector<char> mBuffer;
		VertexLayout mLayout;
	};

#pragma region VertexBufferInlineFunc
	VertexBuffer::VertexBuffer(VertexLayout layout, size_t size)
		: mLayout(std::move(layout))
	{
		Resize(size);
	}

	template<typename ...Params>
	void VertexBuffer::EmplaceBack(Params&&... params)
	{
		assert(sizeof...(params) == mLayout.GetElementCount() && "인자 개수는 Layout 크기와 같아야 함");

		mBuffer.resize(mBuffer.size() + mLayout.GetSize());
		Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
	}
	void VertexBuffer::Resize(size_t newSize)
	{
		const auto size = GetSize();

		if (size < newSize)
		{
			mBuffer.resize(mBuffer.size() + mLayout.GetSize() * (newSize - size));
		}
	}

	Vertex VertexBuffer::Back()
	{
		assert(mBuffer.size() != 0u);
		return Vertex{ mBuffer.data() + mBuffer.size() - mLayout.GetSize(), mLayout };
	}
	Vertex VertexBuffer::Front()
	{
		assert(mBuffer.size() != 0u);
		return Vertex{ mBuffer.data(), mLayout };
	}
	Vertex VertexBuffer::operator[](size_t i)
	{
		assert(i < GetSize());
		return Vertex{ mBuffer.data() + mLayout.GetSize() * i, mLayout };
	}
	ConstVertex VertexBuffer::Back() const
	{
		return const_cast<VertexBuffer*>(this)->Back();
	}
	ConstVertex VertexBuffer::Front() const
	{
		return const_cast<VertexBuffer*>(this)->Front();
	}
	ConstVertex VertexBuffer::operator[](size_t i) const
	{
		return const_cast<VertexBuffer&>(*this)[i];
	}

	const char* VertexBuffer::GetData() const
	{
		return mBuffer.data();
	}
	const VertexLayout& VertexBuffer::GetLayout() const
	{
		return mLayout;
	}
	size_t VertexBuffer::GetSize() const
	{
		return mBuffer.size() / mLayout.GetSize();
	}
	size_t VertexBuffer::GetSizeBytes() const
	{
		return mBuffer.size();
	}
#pragma endregion
}