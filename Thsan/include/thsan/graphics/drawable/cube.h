#pragma once
#include "thsan/core.h"
#include <memory>
#include <thsan/graphics/drawable.h>
#include <thsan/graphics/Transformable.h>

namespace Thsan {

    class THSAN_API Cube: public Drawable, public Transformable {
    public:
        Cube() = default;
        virtual ~Cube() = default;
        virtual void draw(const RenderTarget& target, RenderStates& states) const = 0;
        virtual void setTexture(std::shared_ptr<Texture2D> texture) = 0;

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

        virtual glm::mat4 getTransform() = 0;

        static std::shared_ptr<Cube> create();
    };

    class CubeImpl : public Cube {
    public:
        CubeImpl();
        void draw(const RenderTarget& target, RenderStates& states) const override;

        virtual void setPosition(const glm::vec3& position) override;
        virtual glm::vec3 getPosition() const override;

        virtual void setRotation(const glm::vec3& rotate, float angle) override;
        virtual glm::mat4 getRotation() const override;

        virtual void setScale(const glm::vec3& scale) override;
        virtual glm::vec3 getScale() const override;

        virtual void setOrigin(const glm::vec3& origin) override;
        virtual glm::vec3 getOrigin() const override;

        virtual void move(const glm::vec3& offset) override;
        virtual void scale(const glm::vec3& scaleFactor) override;
        virtual void rotate(const glm::vec3& axis, float angleDegrees) override;

        virtual glm::mat4 getTransform() override;
        void setTexture(std::shared_ptr<Texture2D> texture) override;

        virtual std::shared_ptr<Mesh> getMesh() const override { return mesh; }

    private:
        std::shared_ptr<Texture2D> texture;
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<tsm::Transform> transform;
    };
}
