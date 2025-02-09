#pragma once
#include "thsan/graphics/Mesh.h"
#include "GL/glew.h"

namespace Thsan {
	class GLMesh : public Mesh {
	public:
		GLMesh(uint32_t vertexCount);
		~GLMesh();
		virtual Vertex& operator[](std::size_t index) override;
		virtual const Vertex& operator[](std::size_t index) const override;
		virtual void append(const Vertex& vertex) override;
		virtual void setIndices(const std::vector<uint32_t>& indices) override;
		virtual void setVertices(const std::vector<Vertex>& vertices) override;
		virtual inline std::size_t getVertexCount() const override;
		virtual inline std::size_t getIndiceCount() const override;
		virtual void resize(unsigned int size) override;
		virtual void reserve(unsigned int size) override;
		virtual void setVertexStartOffset(unsigned int offset) override;
		virtual void setVertexEndOffset(unsigned int offset) override;
		virtual size_t size() override;
		virtual void clear() override;
		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void generate() override;
		virtual void generateNoIndices() override;
		virtual void generateTangents() override;
		virtual void generateTangentsNoIndices() override;

		const std::vector<Vertex>& getVertices() const { return vertices; }
		const std::vector<uint32_t>& getIndices() const { return indices; }

		GLuint getVAO() const { return vao; }
		GLuint getVBO() const { return vbo; }
		GLuint getEBO() const { return ebo; }

	private:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		uint32_t vao{0};
		uint32_t vbo{0};
		uint32_t ebo{0};
		uint32_t beginOffset{0};
		uint32_t endOffset{0};
	};
}