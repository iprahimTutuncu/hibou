#include "pch.h"
#include "thsan/graphics/mesh.h"
#include "thsan/graphics/shader.h"
#include "thsan/graphics/texture.h"
#include "thsan/graphics/drawable/cube.h"

Thsan::CubeImpl::CubeImpl()
{
	transform = tsm::Transform::create();

	float size = 1.f;
	mesh = Mesh::create(0);

	std::vector<Vertex> vertices;

	// Vertices
	glm::vec3 front_bottom_left(-size, -size, -size);
	glm::vec3 front_bottom_right(size, -size, -size);
	glm::vec3 front_top_left(-size, size, -size);
	glm::vec3 front_top_right(size, size, -size);
	glm::vec3 back_bottom_left(-size, -size, size);
	glm::vec3 back_bottom_right(size, -size, size);
	glm::vec3 back_top_left(-size, size, size);
	glm::vec3 back_top_right(size, size, size);

	// Normals
	glm::vec3 front_normal(0.0f, 0.0f, -1.0f);
	glm::vec3 back_normal(0.0f, 0.0f, 1.0f);
	glm::vec3 top_normal(0.0f, 1.0f, 0.0f);
	glm::vec3 bottom_normal(0.0f, -1.0f, 0.0f);
	glm::vec3 left_normal(-1.0f, 0.0f, 0.0f);
	glm::vec3 right_normal(1.0f, 0.0f, 0.0f);

	// Front face
	vertices.push_back({ front_bottom_left, front_normal, glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ front_bottom_right, front_normal, glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ front_top_left, front_normal, glm::vec2(0.0f, 1.0f) });
	vertices.push_back({ front_top_right, front_normal, glm::vec2(1.0f, 1.0f) });

	// Back face
	vertices.push_back({ back_bottom_right, back_normal, glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ back_bottom_left, back_normal, glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ back_top_right, back_normal, glm::vec2(0.0f, 1.0f) });
	vertices.push_back({ back_top_left, back_normal, glm::vec2(1.0f, 1.0f) });

	// Top face
	vertices.push_back({ front_top_left, top_normal, glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ front_top_right, top_normal, glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ back_top_left, top_normal, glm::vec2(0.0f, 1.0f) });
	vertices.push_back({ back_top_right, top_normal, glm::vec2(1.0f, 1.0f) });

	// Bottom face
	vertices.push_back({ front_bottom_right, bottom_normal, glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ front_bottom_left, bottom_normal, glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ back_bottom_right, bottom_normal, glm::vec2(0.0f, 1.0f) });
	vertices.push_back({ back_bottom_left, bottom_normal, glm::vec2(1.0f, 1.0f) });

	// Left face
	vertices.push_back({ front_bottom_left, left_normal, glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ front_top_left, left_normal, glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ back_bottom_left, left_normal, glm::vec2(0.0f, 1.0f) });
	vertices.push_back({ back_top_left, left_normal, glm::vec2(1.0f, 1.0f) });

	// Right face
	vertices.push_back({ front_top_right, right_normal, glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ front_bottom_right, right_normal, glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ back_top_right, right_normal, glm::vec2(0.0f, 1.0f) });
	vertices.push_back({ back_bottom_right, right_normal, glm::vec2(1.0f, 1.0f) });

	// Indices
	std::vector<uint32_t> indices = {
		0, 1, 2, 2, 1, 3,
		4, 5, 6, 6, 5, 7,
		8, 9, 10, 10, 9, 11,
		12, 13, 14, 14, 13, 15,
		16, 17, 18, 18, 17, 19,
		20, 21, 22, 22, 21, 23
	};

	mesh->setVertices(vertices);
	mesh->setIndices(indices);

	mesh->generateTangents();
	mesh->generate();
}

void Thsan::CubeImpl::draw(const RenderTarget& target, RenderStates& states) const
{
	auto s = states.getShader().lock();
	s->setMat4("transform", transform->getTransform());
	s->setMat4("rotation", transform->getRotation());
	states.setTexture(TextureType::Default, texture);

	mesh->bind();
	target.draw(*mesh, states);
	mesh->unbind();

	states.unsetTexture(TextureType::Default);
}

void Thsan::CubeImpl::setTexture(std::shared_ptr<Texture2D> texture)
{
	this->texture = texture;
}

std::shared_ptr<Thsan::Cube> Thsan::Cube::create()
{
	return std::make_shared<CubeImpl>();
}

void Thsan::CubeImpl::setPosition(const glm::vec3& position)
{
	transform->setTranslation(position);
}

glm::vec3 Thsan::CubeImpl::getPosition() const
{
	return transform->getTranslation();
}

void Thsan::CubeImpl::setRotation(const glm::vec3& rotate, float angle)
{
	transform->setRotation(rotate, angle);
}

glm::mat4 Thsan::CubeImpl::getRotation() const
{
	return transform->getRotation();
}

void Thsan::CubeImpl::setScale(const glm::vec3& scale)
{
	transform->scale(scale);
}

glm::vec3 Thsan::CubeImpl::getScale() const
{
	return transform->getScale();
}

void Thsan::CubeImpl::setOrigin(const glm::vec3& origin)
{
	transform->setOrigin(origin);
}

glm::vec3 Thsan::CubeImpl::getOrigin() const
{
	return transform->getOrigin();
}

void Thsan::CubeImpl::move(const glm::vec3& offset)
{
	transform->translate(offset);
}

void Thsan::CubeImpl::scale(const glm::vec3& scaleFactor)
{
	transform->scale(scaleFactor);
}

void Thsan::CubeImpl::rotate(const glm::vec3& axis, float angleDegrees)
{
	transform->rotate(axis, angleDegrees);
}
glm::mat4 Thsan::CubeImpl::getTransform()
{
	return transform->getTransform();
}