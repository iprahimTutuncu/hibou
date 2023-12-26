#include "pch.h"
#include "gl_mesh.h"
#include "gl_helper.h"
#include "thsan/log.h"
#include <GL/glew.h>

namespace Thsan {
	GLMesh::GLMesh(uint32_t vertexCount)
	{
		vertices.resize(vertexCount);
		for (uint32_t i = 0; i < vertexCount; i++)
			indices.push_back(i);
		changed = true;
	}
	GLMesh::~GLMesh()
	{
		GL_CHECK(glDeleteBuffers(1, &vbo));
		GL_CHECK(glDeleteVertexArrays(1, &vao));
	}
	Vertex& GLMesh::operator[](std::size_t index)
	{
		TS_CORE_ASSERT(vertices.size() > index, "error: index out of bound for gl_mesh vertices.");
		changed = true;
		return vertices[index];
	}
	const Vertex& GLMesh::operator[](std::size_t index) const
	{
		TS_CORE_ASSERT(vertices.size() > index, "error: index out of bound for gl_mesh vertices.");
		return vertices[index];
	}
	void GLMesh::append(const Vertex& vertex)
	{
		vertices.push_back(vertex);
		changed = true;
	}
	void GLMesh::setIndices(const std::vector<uint32_t>& indices)
	{
		this->indices = indices;
		changed = true;
	}

	void GLMesh::setVertices(const std::vector<Vertex>& vertices)
	{
		this->vertices = vertices;
		changed = true;
	}

	inline std::size_t GLMesh::getVertexCount() const
	{
		return vertices.size();
	}

	inline std::size_t GLMesh::getIndiceCount() const
	{
		return indices.size();
	}

	void GLMesh::resize(unsigned int size) {
		// Ensure that the requested size is greater than zero
		if (size == 0) {
			TS_CORE_WARN("In void GLMesh::resize(unsigned int size), size is equal to zero, should probably be greater than 0.");
		}

		vertices.clear();
		indices.clear();

		vertices.resize(size);

		glDeleteBuffers(1, &vbo);
		glGenBuffers(1, &vbo);

		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * size, nullptr, GL_STATIC_DRAW));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

		changed = true;
	}


	void GLMesh::bind() const
	{
		GL_CHECK(glBindVertexArray(vao));
		GL_CHECK(glEnableVertexAttribArray(0));
		GL_CHECK(glEnableVertexAttribArray(1));
		GL_CHECK(glEnableVertexAttribArray(2));
		GL_CHECK(glEnableVertexAttribArray(3));
		GL_CHECK(glEnableVertexAttribArray(4));
		GL_CHECK(glEnableVertexAttribArray(5));
	}
	void GLMesh::unbind() const
	{
		GL_CHECK(glDisableVertexAttribArray(0));
		GL_CHECK(glDisableVertexAttribArray(1));
		GL_CHECK(glDisableVertexAttribArray(2));
		GL_CHECK(glDisableVertexAttribArray(3));
		GL_CHECK(glDisableVertexAttribArray(4));
		GL_CHECK(glDisableVertexAttribArray(5));
		GL_CHECK(glBindVertexArray(0));
	}
	void GLMesh::generate()
	{
		if(!changed)
			return;

		GL_CHECK(glGenVertexArrays(1, &vao));
		GL_CHECK(glGenBuffers(1, &vbo));
		GL_CHECK(glGenBuffers(1, &ebo));

		GL_CHECK(glBindVertexArray(vao));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));

		if (vertices.size() == 0) {
			TS_CORE_WARN("warning: GLMesh::generate() generating a mesh with no vertices.");
			GL_CHECK(glBindVertexArray(0));
			return;
		}

		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
		GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

		GL_CHECK(glEnableVertexAttribArray(0));
		GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

		GL_CHECK(glEnableVertexAttribArray(1));
		GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

		GL_CHECK(glEnableVertexAttribArray(2));
		GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord)));

		GL_CHECK(glEnableVertexAttribArray(3));
		GL_CHECK(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent)));

		GL_CHECK(glEnableVertexAttribArray(4));
		GL_CHECK(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent)));

		GL_CHECK(glEnableVertexAttribArray(5));
		GL_CHECK(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color)));

		GL_CHECK(glBindVertexArray(0));

		changed = false;
	}
}