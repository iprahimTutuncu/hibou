#pragma once
#include "thsan/core.h"
#include "thsan/graphics/vertex.h"
#include <cstdint>
#include <memory>

namespace Thsan {
	class THSAN_API Mesh {
	public:
		Mesh() = default;
		virtual ~Mesh() = default;
		virtual Vertex& operator[](std::size_t index) = 0;
		virtual const Vertex& operator[](std::size_t index) const = 0;
		virtual void append(const Vertex& vertex) = 0;
		virtual void setIndices(const std::vector<uint32_t>& indices) = 0;
		virtual void setVertices(const std::vector<Vertex>& vertices) = 0;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void resize(unsigned int size) = 0;
		virtual inline std::size_t getVertexCount() const = 0;
		virtual inline std::size_t getIndiceCount() const = 0;
		inline bool hasChanged() { return changed; };
		virtual void generate() = 0;

	protected:
		bool changed{false};

	};

	THSAN_API std::shared_ptr<Mesh> create_mesh(uint32_t vertexCount);

}
