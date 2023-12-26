#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <thsan/system/vector.h>
#include <tsm/math/color.h>
#include <string>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_uint2.hpp>
#include "drawable.h"
#include "Transformable.h"

namespace tsm {
    class Transform;
}

namespace Thsan {
    
    class Texture2D;

    class THSAN_API SpriteAnimation : public Drawable, public Transformable {
    public:
        virtual void hide() = 0;
        virtual void show() = 0;
        virtual bool isHidden() const = 0;
        virtual void flipX() = 0;
        virtual void flipY() = 0;
        virtual bool isFlipX() const = 0;
        virtual bool isFlipY() const = 0;
        virtual void setTexture(std::shared_ptr<Texture2D> texture) = 0;
        virtual void setCurrAnimation(std::string name) = 0;
        virtual void add(glm::vec4 rect, float time) = 0;
        virtual void enableLoop() = 0;
        virtual void disableLoop() = 0;
        virtual void start(unsigned int from = 0, unsigned int to = 0) = 0;
        virtual void stop() = 0;
        virtual void reset() = 0;
        virtual void restart(unsigned int from = 0, unsigned int to = 0) = 0; //will call start() directly after reset()
        virtual bool isFinished() const = 0;
        virtual void setSpeed(float speed_factor) = 0;

        virtual bool hasAnimation(const std::string& name) const = 0;
        virtual unsigned int getFrameIndex() = 0;
        virtual int getFrameCount() = 0;
        virtual int getFrameWidth() = 0;
        virtual int getFrameHeight() = 0;
        virtual float getCurrentAnimationTimeDuration() = 0;
        virtual float getCurrentAnimationTimeReached() const = 0;
        virtual glm::uvec2 getSpriteSheetSize() const  = 0;
        virtual void update(const float& dt) = 0;

        virtual void setColor(tsm::Color color) = 0;
        virtual tsm::Color getColor() const = 0;

        virtual void setKeyColor(tsm::Color color) = 0;
        virtual tsm::Color getKeyColor() const = 0;


        virtual ~SpriteAnimation() = default;

        static std::shared_ptr<SpriteAnimation> create();
        static std::shared_ptr<SpriteAnimation> create(std::shared_ptr<Texture2D> texture);

        //inherited from drawable
        virtual void draw(const RenderTarget& target, RenderStates2D& states) const = 0;

        //inherited from transformable
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

    class AnimationData;

    class SpriteAnimationImpl : public SpriteAnimation {
    public:
        SpriteAnimationImpl(); // Constructor

        void hide() override;
        void show() override;
        bool isHidden() const override;
        void flipX() override;
        void flipY() override;
        bool isFlipX() const override;
        bool isFlipY() const override;
        void setTexture(std::shared_ptr<Texture2D> texture) override;
        void setCurrAnimation(std::string name) override;
        void add(glm::vec4 rect, float time) override;
        void enableLoop() override;
        void disableLoop() override;
        void start(unsigned int from = 0, unsigned int to = 0) override;
        void stop() override;
        void reset() override;
        void restart(unsigned int from = 0, unsigned int to = 0) override;
        bool isFinished() const override;
        void setSpeed(float speed_factor) override;
        bool hasAnimation(const std::string& name) const override;
        unsigned int getFrameIndex() override;
        int getFrameCount() override;
        int getFrameWidth() override;
        int getFrameHeight() override;
        float getCurrentAnimationTimeDuration() override;
        float getCurrentAnimationTimeReached() const override;
        glm::uvec2 getSpriteSheetSize() const override;
        void update(const float& dt) override;

        void setColor(tsm::Color color) override;
        tsm::Color getColor() const override;

        void setKeyColor(tsm::Color color) override;
        tsm::Color getKeyColor() const override;

        ~SpriteAnimationImpl() = default;

        //inherited from drawable
        void draw(const RenderTarget& target, RenderStates2D& states) const override;

        //inherited from transformable
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


        virtual glm::mat4 getTranform() override;

    private:
        void generate();
        std::unordered_map<std::string, std::unique_ptr<AnimationData>> animations;
        std::shared_ptr<Texture2D> texture;
        int xFlip{ 1 };
        int yFlip{ 1 };
        bool hidden{ false };
        bool stopped{ true };

        unsigned int from;
        unsigned int to;
        
        float speedFactor{ 1.f };

        std::string curr{};

        bool animationIsDone{ true };
        bool onNewFrame{ false };

        float tempsAccumuleFrame{0.f};
        float tempsAccumuleAnimation{0.f};

        float spritesheet_width;
        float spritesheet_height;

        tsm::Color keyColor;
        tsm::Color color;

        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<tsm::Transform> transform;

    };
}