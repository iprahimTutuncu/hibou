#include "pch.h"
#include "fixed_target_camera.h"

namespace tsm {
	void FixedTargetCamera::zoom(float amount)
	{
		position += front * amount;
		distance = glm::distance(position, target);
		distance = std::max(min_distance, std::min(distance, max_distance));
		update();
	}

	void FixedTargetCamera::pan(float dx, float dy)
	{
		glm::vec3 tx = right * dx;
		glm::vec3 ty = up * dy;
		position += tx + ty;
		target += tx + ty;
		update();
	}

	void FixedTargetCamera::move(float dx, float dz)
	{
		glm::vec3 tx = right * dx;
		glm::vec3 tz = front * dz;
		position += tx + tz;
		target += tx + tz;
		update();
	}
	void FixedTargetCamera::setMinDistance(float min)
	{
		min_distance = min;
	}

	void FixedTargetCamera::setMaxDistance(float max)
	{
		max_distance = max;
	}

	void FixedTargetCamera::update()
	{
		glm::mat4 R = glm::yawPitchRoll(yaw_value, pitch_value, roll_value);
		glm::vec3 T = glm::vec3(0.f, 0.f, distance);
		T = glm::vec3(R * glm::vec4(T, 0.0f));
		position = target + T;
		front = glm::normalize(target - position);
		up = glm::vec3(R * glm::vec4(UP, 0.0f));
		right = glm::cross(front, up);
		view = glm::lookAt(position, target, up);
	}

	void FixedTargetCamera::setTarget(const glm::vec3& target)
	{
		this->target = target;
		distance = glm::distance(position, target);
		distance = std::max(min_distance, std::min(distance, max_distance));
		view = glm::lookAt(position, target, up);

		yaw_value = 0.f;
		pitch_value = 0.f;


		if (view[0][0] < 0)
			yaw_value = glm::degrees((float)(3.14159 - asinf(-view[2][0])));
		else
			yaw_value = glm::degrees(asinf(-view[2][0]));

		pitch_value = glm::degrees(asinf(-view[1][2]));
	}

	const glm::vec3 FixedTargetCamera::getTarget() const
	{
		return target;
	}
}