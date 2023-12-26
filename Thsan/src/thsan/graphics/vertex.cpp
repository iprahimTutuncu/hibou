#include "pch.h"
#include "vertex.h"
#include "thsan/log.h"
/*
namespace Thsan
{
	VertexImpl::VertexImpl()
	{
	}
	VertexImpl::~VertexImpl()
	{
	}

	glm::vec3 VertexImpl::getAttribute(Vertex::TypeVec3 type)
	{
		switch (type)
		{
		case Thsan::Vertex::TypeVec3::position:
			return vertex_data.position;
		case Thsan::Vertex::TypeVec3::normal:
			return vertex_data.normal;
		case Thsan::Vertex::TypeVec3::tangent:
			return vertex_data.tangent;
		case Thsan::Vertex::TypeVec3::bitangent:
			return vertex_data.bitangent;
		default:
			break;
		}

		TS_CORE_ERROR("error: type of Vertex::TypeVec3 not a member of VertexImpl");
	}
	glm::vec2 VertexImpl::getAttribute(Vertex::TypeVec2 type)
	{
		TS_CORE_ERROR("error: type of Vertex::TypeVec2 not a member of VertexImpl");
	}

	int VertexImpl::getAttributeOffset(TypeVec3 type) 
	{
		switch (type)
		{
		case Thsan::Vertex::TypeVec3::position:
			return offsetof(VertexData, position);
		case Thsan::Vertex::TypeVec3::normal:
			return offsetof(VertexData, normal);
		case Thsan::Vertex::TypeVec3::tangent:
			return offsetof(VertexData, tangent);
		case Thsan::Vertex::TypeVec3::bitangent:
			return offsetof(VertexData, bitangent);
		default:
			break;
		}

		TS_CORE_ERROR("error: type of Vertex::TypeVec3 not a member of VertexImpl");
		return -1;

	}
	int VertexImpl::getAttributeOffset(TypeVec2 type) 
	{
		switch (type)
		{
		case Thsan::Vertex::TypeVec2::texCoord:
			return offsetof(VertexData, texCoord);
		default:
			break;
		}

		TS_CORE_ERROR("error: type of Vertex::TypeVec2 not a member of VertexImpl");
		return -1;
	}
	int VertexImpl::getVertexDataSize()
	{
		return sizeof(VertexData);
	}
	const void* VertexImpl::getData()
	{
		return &vertex_data;
	}
}
*/