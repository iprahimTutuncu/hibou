#pragma once
#include "thsan/graphics/Mesh.h"

namespace Thsan {
	class GLMesh : public Mesh {
	public:
		GLMesh(uint32_t vertexCount);
		~GLMesh();
		Vertex& operator[](std::size_t index) override;
		const Vertex& operator[](std::size_t index) const override;
		void append(const Vertex& vertex) override;
		void setIndices(const std::vector<uint32_t>& indices) override;
		void setVertices(const std::vector<Vertex>& vertices) override;
		inline std::size_t getVertexCount() const override;
		inline std::size_t getIndiceCount() const override;
		void resize(unsigned int size) override;
		void bind() const override;
		void unbind() const override;
		void generate() override;

	private:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		uint32_t vao{0};
		uint32_t vbo{0};
		uint32_t ebo{0};
	};
}