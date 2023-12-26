#include "pch.h"
#include "free_camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include <math.h>

namespace tsm {
	void FreeCameraImpl::moveForward(float amount)
	{
		translation += amount * front;
	}

	void FreeCameraImpl::moveSide(float amount)
	{
		translation += amount * right;
	}

	void FreeCameraImpl::moveAbove(float amount)
	{
		translation += amount * up;
	}

	void FreeCameraImpl::setProjection(const float FOV, const float aspect_ratio, const float near_plane, const float far_plane)
	{
		this->FOV = FOV;
		this->aspect_ratio = aspect_ratio;
		this->near_plane = near_plane;
		this->far_plane = far_plane;

		projection = glm::perspective(glm::radians(FOV), aspect_ratio, near_plane, far_plane);
	}

	void FreeCameraImpl::setPosition(const glm::vec3& position)
	{
		this->position = position;
	}

	glm::mat4 FreeCameraImpl::getViewProjection()
	{
		return projection * view;
	}

	glm::mat4 FreeCameraImpl::getProjection()
	{
		return projection;
	}

	glm::mat4 FreeCameraImpl::getView()
	{
		return view;
	}

	const float FreeCameraImpl::GetFOV() const
	{
		return FOV;
	}

	const float FreeCameraImpl::GetAspectRatio() const
	{
		return aspect_ratio;
	}

	const float FreeCameraImpl::getNearPlane() const
	{
		return near_plane;
	}

	const float FreeCameraImpl::getFarPlane() const
	{
		return far_plane;
	}

	const glm::vec3 FreeCameraImpl::getPosition() const
	{
		return position;
	}

	const glm::vec3 FreeCameraImpl::getFront() const
	{
		return front;
	}

	std::shared_ptr<FreeCameraImpl> FreeCameraImpl::create()
	{
		return std::make_shared<FreeCameraImpl>();
	}

	void FreeCameraImpl::update()
	{
		glm::mat4 R = glm::yawPitchRoll(yaw_value, pitch_value, roll_value);
		front = glm::vec3(R * glm::vec4(0.f, 0.f, 1.f, 0.f));
		up = glm::vec3(R * glm::vec4(0.f, 1.f, 0.f, 0.f));
		position += translation;
		glm::vec3 target = position + front;
		right = glm::cross(front, up);
		view = glm::lookAt(position, target, up);
	}

	void FreeCameraImpl::roll(float rotation)
	{
		roll_value += rotation;
	}
	void FreeCameraImpl::pitch(float rotation)
	{
		pitch_value += rotation;
	}

	void FreeCameraImpl::yaw(float rotation)
	{
		yaw_value += rotation;
	}
	void FreeCameraImpl::rotate(const float yaw, const float pitch, const float roll)
	{
		yaw_value	+= yaw;
		pitch_value += pitch;
		roll_value	+= roll;
	}
	void FreeCameraImpl::setYaw(float rotation)
	{
		this->yaw_value = rotation;
	}
	void FreeCameraImpl::setRoll(float rotation)
	{
		this->roll_value = rotation;
	}
	void FreeCameraImpl::setPitch(float rotation)
	{
		this->pitch_value = rotation;
	}
	void FreeCameraImpl::setRotation(const float yaw, const float pitch, const float roll)
	{
		yaw_value	= yaw;
		pitch_value = pitch;
		roll_value	= roll;
	}

	std::shared_ptr<FreeCamera> FreeCamera::create()
	{
		return std::make_shared<FreeCameraImpl>();
	}

}
