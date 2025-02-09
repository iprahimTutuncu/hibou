#pragma once
#include "abstract_camera.h"
#include "thsan/core.h"

namespace tsm {

    class THSAN_API TargetCamera: public AbstractCamera {
    public:
        TargetCamera() = default;
        virtual ~TargetCamera() = default;

        virtual void setTarget(const glm::vec3& target) = 0;
        virtual const glm::vec3 getTarget() const = 0;

        virtual void zoom(float amount) = 0;
        virtual void pan(float dx, float dy) = 0;
        virtual void move(float dx, float dy) = 0;

        // Inherited via AbstractCamera
        virtual void update() = 0;
    };

    class THSAN_API TargetCameraImpl : public TargetCamera {
    public:
        TargetCameraImpl() = default;
        virtual ~TargetCameraImpl() = default;

        void setTarget(const glm::vec3& target) override;
        const glm::vec3 getTarget() const override;

        void zoom(float amount) override;
        void pan(float dx, float dy) override;
        void move(float dx, float dy) override;

        // Inherited via AbstractCamera
        void update() override;

    protected:
        glm::vec3 target;

        float min_Ry = -60.f;
        float max_Ry = 60.f;

        float distance{ 10.f };
        float min_distance{ 0.1f };
        float max_distance{ 100.f };
    };
}

