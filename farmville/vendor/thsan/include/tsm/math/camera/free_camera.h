#pragma once
#include "abstract_camera.h"
#include "thsan/core.h"

namespace tsm {
	class THSAN_API FreeCamera: public AbstractCamera{
        
	public:
        FreeCamera() = default;
        ~FreeCamera() = default;

       virtual void moveForward(float amount) = 0;
       virtual void moveSide(float amount) = 0;
       virtual void moveAbove(float amount) = 0;

        // Hérité via AbstractCamera
        virtual void update() = 0;
        virtual void yaw(float rotation) = 0;
        virtual void roll(float rotation) = 0;
        virtual void pitch(float rotation) = 0;
        virtual void rotate(const float yaw, const float pitch, const float roll) = 0;
        virtual void setYaw(float rotation) = 0;
        virtual void setRoll(float rotation) = 0;
        virtual void setPitch(float rotation) = 0;
        virtual void setRotation(const float yaw, const float pitch, const float roll) = 0;
        virtual void setProjection(const float FOV, const float aspect_ratio, const float near_plane, const float far_plane) = 0;
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

        static std::shared_ptr<FreeCamera> create();
    };

    class FreeCameraImpl : public FreeCamera {

    public:
        FreeCameraImpl() = default;
        ~FreeCameraImpl() = default;

        virtual void moveForward(float amount) override;
        virtual void moveSide(float amount) override;
        virtual void moveAbove(float amount) override;

        virtual void update() override;
        virtual void yaw(float rotation) override;
        virtual void roll(float rotation) override;
        virtual void pitch(float rotation) override;
        virtual void rotate(const float yaw, const float pitch, const float roll) override;
        virtual void setYaw(float rotation) override;
        virtual void setRoll(float rotation) override;
        virtual void setPitch(float rotation) override;
        virtual void setRotation(const float yaw, const float pitch, const float roll) override;
        virtual void setProjection(const float FOV, const float aspect_ratio, const float near_plane, const float far_plane) override;
        virtual void setPosition(const glm::vec3& position) override;
        virtual glm::mat4 getViewProjection() override;
        virtual glm::mat4 getProjection() override;
        virtual glm::mat4 getView() override;
        virtual const float GetFOV() const override;
        virtual const float GetAspectRatio() const override;
        virtual const float getNearPlane() const override;
        virtual const float getFarPlane() const override;
        virtual const glm::vec3 getPosition() const override;
        virtual const glm::vec3 getFront() const override;

        static std::shared_ptr<FreeCameraImpl> create();

    private:
        static glm::vec3 UP;

        glm::vec3 up{ glm::vec3(0.f, 1.0f, 0.f) };
        glm::vec3 front{ glm::vec3(0.f, 0.f, 1.f) };
        glm::vec3 right{ glm::vec3(0.f, 0.f, 0.f) };

        glm::vec3 position{ glm::vec3(0.f, 0.f, 0.f) };

        glm::mat4 projection{ glm::mat4() };
        glm::mat4 view{ glm::mat4() };

        float aspect_ratio{ 1.0f };
        float near_plane{ 0.01f };
        float far_plane{ 1000.f };
        float FOV{ 53.f };

        float yaw_value{ 0.f };
        float roll_value{ 0.f };
        float pitch_value{ 0.f };

        glm::vec3 translation{ glm::vec3(0.f, 0.f, 0.f) };


    };
}