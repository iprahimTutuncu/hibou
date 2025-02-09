#pragma once
#include "thsan/core.h"
#include "thsan/graphics/vertex.h"
#include <cstdint>
#include <memory>

namespace Thsan {

	enum class PrimitiveType {
		Point,
		Lines,
		Triangles,
		Quads,
		CustomIndices
	};

	class THSAN_API Mesh {
	public:
		Mesh() = default;
		virtual ~Mesh() = default;

		static std::shared_ptr<Mesh> create(uint32_t vertexCount);

		virtual Vertex& operator[](std::size_t index) = 0;
		virtual const Vertex& operator[](std::size_t index) const = 0;
		virtual void append(const Vertex& vertex) = 0;
		virtual void setIndices(const std::vector<uint32_t>& indices) = 0;
		virtual void setVertices(const std::vector<Vertex>& vertices) = 0;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void resize(unsigned int size) = 0;
		virtual void reserve(unsigned int size) = 0;
		virtual void setVertexStartOffset(unsigned int offset) = 0;
		virtual void setVertexEndOffset(unsigned int offset) = 0;
		virtual size_t size() = 0;
		virtual void clear() = 0;
		virtual inline std::size_t getVertexCount() const = 0;
		virtual inline std::size_t getIndiceCount() const = 0;
		inline bool hasChanged() const { return changed; };

		virtual void generate() = 0;
		virtual void generateNoIndices() = 0;
		virtual void generateTangents() = 0;
		virtual void generateTangentsNoIndices() = 0;

		inline PrimitiveType getPrimitiveType() const { return primitiveType; }
		virtual void setPrimitiveType(PrimitiveType type) { primitiveType = type; }


	protected:

		bool changed{ false };
		PrimitiveType primitiveType{ PrimitiveType::Triangles };

	};


}
