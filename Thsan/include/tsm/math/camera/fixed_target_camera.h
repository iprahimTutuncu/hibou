#pragma once
#include "target_camera.h"
#include <vector>
#include "thsan/core.h"

namespace tsm {
	class THSAN_API FixedTargetCamera : public TargetCamera {
	public:
		FixedTargetCamera() = default;
		~FixedTargetCamera() = default;

		// Inherited via AbstractCamera
		virtual void update() override;

		// Inherited via TargetCamera
		virtual void zoom(float amount) override;

		virtual void pan(float dx, float dy) override;

		virtual void move(float dx, float dz) override;

		void setMinDistance(float min);
		void setMaxDistance(float max);

		virtual void setTarget(const glm::vec3& target) override;
		virtual const glm::vec3 getTarget() const override;

	private:

	};
}