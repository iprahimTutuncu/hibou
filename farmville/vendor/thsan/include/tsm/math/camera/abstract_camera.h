#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "thsan/core.h"

namespace tsm {

	class THSAN_API AbstractCamera {
	public:
		AbstractCamera() = default;
		virtual ~AbstractCamera() = default;

		virtual void update() = 0;
		virtual void yaw(float rotation) = 0;
		virtual void roll(float rotation) = 0;
		virtual void pitch(float rotation) = 0;
		virtual void rotate(const float yaw, const float pitch, const float roll) = 0;

		virtual void setYaw(float rotation) = 0;
		virtual void setRoll(float rotation) = 0;
		virtual void setPitch(float rotation) = 0;
		virtual void setRotation(const float yaw, const float pitch, const float roll) = 0;

		virtual void setProjection(const float FOV, const float aspect_ratio, const float near_plane = 0.1f, const float far_plane = 1000.f) = 0;
		virtual void setPosition(const glm::vec3& position) = 0;

		virtual glm::mat4 getViewProjection() = 0;
		virtual glm::mat4 getProjection() = 0;
		virtual glm::mat4 getView() = 0;

		virtual const float GetFOV() const = 0;
		virtual const float GetAspectRatio() const = 0;
		virtual const float getNearPlane() const = 0;
		virtual const float getFarPlane() const = 0;

		virtual const glm::vec3 getPosition() const = 0;
		virtual const glm::vec3 getFront() const = 0;
	};
}