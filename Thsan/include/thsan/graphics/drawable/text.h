#pragma once
#include <string>
#include "../drawable.h"
#include "../transformable.h"
#include "tsm/math/color.h"

namespace Thsan {

    class Font;

    class THSAN_API Text : public Drawable, public Transformable {
    public:
        Text() = default;
        virtual ~Text() = default;
        virtual void setString(const std::u32string& text) = 0;
        virtual const std::u32string getString() const = 0;

        virtual void draw(const RenderTarget& target, RenderStates& states) const = 0;
        virtual void setFont(std::shared_ptr<Font> font) = 0;

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
        virtual std::shared_ptr<Mesh> getMesh() const = 0;

        static std::shared_ptr<Text> create();

        // will require a setString() if you want to see the modification
        // color is done per vertex on the geometry itself. Do not attempt flickering of text color.
        virtual void setColor(tsm::Color color) = 0;
        virtual tsm::Color getColor() = 0;

        //this is independant of scaling, multiply the scaling yourself if you need to
        //todo in 2000 years, getTextScaledHeight()
        virtual float getTextWidth() const = 0;

        //this is independant of scaling, multiply the scaling yourself if you need to
        //todo in 2000 years, getTextScaledHeight()
        virtual float getTextHeight() const = 0;
    };

    class TextImpl: public Text 
    {
    public:
        TextImpl();
        // Override from Text
        void setString(const std::u32string& text) override;
        const std::u32string getString() const override;

        void draw(const RenderTarget& target, RenderStates& states) const override;
        void setFont(std::shared_ptr<Font> font) override;

        void setPosition(const glm::vec3& position) override;
        glm::vec3 getPosition() const override;

        void setRotation(const glm::vec3& rotate, float angle) override;
        glm::mat4 getRotation() const override;

        void setScale(const glm::vec3& scale) override;
        glm::vec3 getScale() const override;

        void setOrigin(const glm::vec3& origin) override;
        glm::vec3 getOrigin() const override;

        void move(const glm::vec3& offset) override;
        void scale(const glm::vec3& scaleFactor) override;
        void rotate(const glm::vec3& axis, float angleDegrees) override;

        std::shared_ptr<Mesh> getMesh() const override;


        glm::mat4 getTransform() override;

        void setColor(tsm::Color color) override;
        tsm::Color getColor() override;

        float getTextWidth() const override;
        float getTextHeight() const override;

    private:
        int textWidth{-1};
        int textHeight{-1};
        std::shared_ptr<Mesh> mesh;
        std::u32string text;
        std::shared_ptr<Font> pFont;
        std::shared_ptr<tsm::Transform> transform;
        glm::vec4 fontColor;
    };
}