#pragma once

#include "thsan/core.h"
#include "glm/glm.hpp"

namespace Thsan {

    class THSAN_API View {
    public:
        virtual ~View() = default;

        virtual void setPosition(float x, float y) = 0;
        virtual void setSize(float width, float height) = 0;
        virtual void setZoom(float zoom) = 0;
        virtual void setRotation(float angle) = 0;

        virtual glm::vec2 getPosition() = 0;
        virtual glm::vec2 getSize() = 0;
        virtual float getZoom() = 0;
        virtual float getRotation() = 0;

        virtual glm::mat4 getViewMatrix() const = 0;
        virtual glm::mat4 getProjectionMatrix() const = 0;
        virtual glm::mat4 getViewProjectionMatrix() const = 0;

        static std::shared_ptr<View> create(float x, float y, float width, float height, float zoom = 1.0f, float rotation = 0.0f);
    };

    class ViewImpl : public View {
    public:
        ViewImpl(float x, float y, float width, float height, float zoom = 1.0f, float rotation = 0.0f);

        void setPosition(float x, float y) override;
        void setSize(float width, float height) override;
        void setZoom(float zoom) override;
        void setRotation(float angle) override;

        glm::vec2 getPosition() override;
        glm::vec2 getSize() override;
        float getZoom() override;
        float getRotation() override;

        glm::mat4 getViewMatrix() const override;
        glm::mat4 getProjectionMatrix() const override;
        glm::mat4 getViewProjectionMatrix() const override;

    private:
        glm::vec2 position;
        glm::vec2 size;
        float zoomFactor;
        float rotationAngle;
    };

}
