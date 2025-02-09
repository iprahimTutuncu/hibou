#include "pch.h"
#include "platform/openGL/gl_mesh.h"
#include "platform/openGL/gl_helper.h"
#include "thsan/log.h"
#include <GL/glew.h>
#include <glm/fwd.hpp>

namespace Thsan {
	GLMesh::GLMesh(uint32_t vertexCount)
	{
		vertices.resize(vertexCount);
		endOffset = vertexCount;
		indices.clear();
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
		endOffset = vertices.size();
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
		endOffset = vertices.size();
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


		if (size == 0) {
			TS_CORE_WARN("In GLMesh::resize(unsigned int size): size is equal to zero; it should probably be greater than 0.");
			return;
		}

		vertices.clear();
		indices.clear();

		vertices.resize(size);

		// Generate a new VBO
		glDeleteBuffers(1, &vbo);
		glGenBuffers(1, &vbo);

		// Bind the new VBO and allocate space
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * size, nullptr, GL_DYNAMIC_DRAW));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

		changed = true;
		
		endOffset = vertices.size();
	}

	void GLMesh::reserve(unsigned int size)
	{
		// Ensure that the requested size is greater than zero
		if (size == 0) {
			TS_CORE_WARN("In GLMesh::resize(unsigned int size): size is equal to zero; it should probably be greater than 0.");
			return;
		}

		if (size > vertices.size())
			vertices.reserve(size);
		
	}

	void GLMesh::setVertexStartOffset(unsigned int offset)
	{
		if (offset > endOffset) {
			beginOffset = endOffset;
			TS_CORE_WARN("GLMesh::setVertexStartOffset, offset bigger than end offset, beginOffset is set to endOffset");
		}
		else
			endOffset = offset;
	}

	void GLMesh::setVertexEndOffset(unsigned int offset)
	{
		if (offset < beginOffset) {
			endOffset = beginOffset;
			TS_CORE_WARN("GLMesh::setVertexEndOffset, offset smaller than begin offset, endOffset is set to beginOffset");
		}
		else
			endOffset = offset;
	}

	size_t GLMesh::size()
	{
		return vertices.size();
	}

	void GLMesh::clear()
	{
		vertices.clear();
		indices.clear();
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
		GL_CHECK(glEnableVertexAttribArray(6));
	}
	void GLMesh::unbind() const
	{
		GL_CHECK(glDisableVertexAttribArray(0));
		GL_CHECK(glDisableVertexAttribArray(1));
		GL_CHECK(glDisableVertexAttribArray(2));
		GL_CHECK(glDisableVertexAttribArray(3));
		GL_CHECK(glDisableVertexAttribArray(4));
		GL_CHECK(glDisableVertexAttribArray(5));
		GL_CHECK(glDisableVertexAttribArray(6));
		GL_CHECK(glBindVertexArray(0));
	}
	void GLMesh::generate()
	{
		if (vao == 0)
			GL_CHECK(glGenVertexArrays(1, &vao));

		if (vbo == 0)
			GL_CHECK(glGenBuffers(1, &vbo));

		if (ebo == 0)
			GL_CHECK(glGenBuffers(1, &ebo));

		if (!changed)
			return;
		else
			changed = false;

		bind();

		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, (endOffset - beginOffset) * sizeof(Vertex), &vertices[beginOffset], GL_DYNAMIC_DRAW));
		GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_DYNAMIC_DRAW));

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

		GL_CHECK(glEnableVertexAttribArray(6));
		GL_CHECK(glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, externalPosition)));

		unbind();


		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void GLMesh::generateNoIndices()
	{
		if (vao == 0)
			GL_CHECK(glGenVertexArrays(1, &vao));

		if (vbo == 0)
			GL_CHECK(glGenBuffers(1, &vbo));

		if (!changed)
			return;
		else
			changed = false;

		bind();

		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));

		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, (endOffset - beginOffset) * sizeof(Vertex), &vertices[beginOffset], GL_DYNAMIC_DRAW));

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

		GL_CHECK(glEnableVertexAttribArray(6));
		GL_CHECK(glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, externalPosition)));

		unbind();

		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void GLMesh::generateTangents()
	{
		for (size_t i = 0; i < indices.size(); i += 3) {

			uint32_t index1 = indices[i];
			uint32_t index2 = indices[i + 1];
			uint32_t index3 = indices[i + 2];

			Vertex& v1 = vertices[index1];
			Vertex& v2 = vertices[index2];
			Vertex& v3 = vertices[index3];

			glm::vec3 edge1 = v2.position - v1.position;
			glm::vec3 edge2 = v3.position - v1.position;
			glm::vec2 deltaUV1 = v2.texCoord - v1.texCoord;
			glm::vec2 deltaUV2 = v3.texCoord - v1.texCoord;

			glm::vec3 tangent, bitangent;
			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

			v1.tangent += tangent;
			v2.tangent += tangent;
			v3.tangent += tangent;
			v1.bitangent += bitangent;
			v2.bitangent += bitangent;
			v3.bitangent += bitangent;
		}

		for (auto& vertex : vertices) {
			vertex.tangent = glm::normalize(vertex.tangent);
			vertex.bitangent = glm::normalize(vertex.bitangent);
		}
	}
	void GLMesh::generateTangentsNoIndices()
	{
		for (size_t i = 0; i < vertices.size(); i += 3) {

			Vertex& v1 = vertices[i];
			Vertex& v2 = vertices[i + 1];
			Vertex& v3 = vertices[i + 2];

			glm::vec3 edge1 = v2.position - v1.position;
			glm::vec3 edge2 = v3.position - v1.position;
			glm::vec2 deltaUV1 = v2.texCoord - v1.texCoord;
			glm::vec2 deltaUV2 = v3.texCoord - v1.texCoord;

			glm::vec3 tangent, bitangent;
			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

			v1.tangent += tangent;
			v2.tangent += tangent;
			v3.tangent += tangent;
			v1.bitangent += bitangent;
			v2.bitangent += bitangent;
			v3.bitangent += bitangent;
		}

		for (auto& vertex : vertices) {
			vertex.tangent = glm::normalize(vertex.tangent);
			vertex.bitangent = glm::normalize(vertex.bitangent);
		}
	}
}