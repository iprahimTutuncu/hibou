#pragma once

#include "thsan/core.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Thsan {

    class THSAN_API Transformable {
    public:
        virtual ~Transformable() = default;

        virtual void setPosition(const glm::vec3& position) = 0;
        virtual glm::vec3 getPosition() const = 0;

        virtual void setRotation(const glm::vec3& rotate, float angle) = 0;
        virtual glm::mat4 getRotation() const = 0;

        virtual void setScale(const glm::vec3& scale) = 0;
        virtual glm::vec3 getScale() const = 0;

        virtual void setOrigin(const glm::vec3& origin) = 0;
        virtual glm::vec3 getOrigin() const = 0;

        virtual void move(const glm::vec3& offset) = 0;
        virtual void scale(const glm::vec3& scaleFactor) = 0;
        virtual void rotate(const glm::vec3& axis, float angleDegrees) = 0;

        virtual glm::mat4 getTranform() = 0;
    };

}
