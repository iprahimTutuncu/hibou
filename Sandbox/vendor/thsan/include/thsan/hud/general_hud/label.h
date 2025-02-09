#pragma once
#include "widget.h"
#include <tsm/math/color.h>
#include <functional>

namespace tsm
{
    class Color;
}

namespace Thsan
{
    class Font;
    class Text;
}

namespace Thsan::UI {

    class THSAN_API Label : public Widget
    {
    public:
        virtual void setString(const std::u32string& text) = 0;
        virtual const std::u32string getString() const = 0;
        virtual void setFont(std::shared_ptr<Thsan::Font> font) = 0;
        virtual void enableCustomFont() = 0;
        virtual void disableCustomFont() = 0;
        virtual void setCustomFont(std::shared_ptr<Thsan::Font> font) = 0;
        virtual void setColor(tsm::Color color) = 0;
        virtual void setSizeToFontSize() = 0;
        virtual glm::ivec2 getFontSize() = 0;
        virtual tsm::Color getColor() const = 0;

        static std::shared_ptr<Label> create(std::string name);
    };

    class LabelImpl : public Label
    {
    public:
        LabelImpl(std::string name);
        ~LabelImpl() = default;

        virtual void setString(const std::u32string& text) override;
        virtual const std::u32string getString() const override;

        void enableCustomFont() override;
        void disableCustomFont() override;

        void setSizeToFontSize() override;
        glm::ivec2 getFontSize() override;

        void setFont(std::shared_ptr<Thsan::Font> font) override;
        void setCustomFont(std::shared_ptr<Thsan::Font> font) override;

        virtual void setColor(tsm::Color color) override;
        virtual tsm::Color getColor() const override;

        // Implement abstract methods from Widget
        virtual void update(float deltaTime) override;

        void remove(const std::shared_ptr<Widget>& widget) override;

        void setMargin(glm::vec2 margin) override;
        void setSize(glm::vec2 size) override;
        void setAnchor(Anchor anchor) override;

        glm::vec2 getMargin() const override;
        glm::vec2 getGlobalPosition() const override;
        glm::vec2 getSize() const override;
        Anchor getAnchor() const override;

        void hide() override;
        void show() override;
        bool isHidden() const override;

        WidgetData& getWidgetData() override;

        std::string getName() override;
        std::shared_ptr<Mesh> getMesh() const override;


    protected:
        void draw(const RenderTarget& target, RenderStates& states) const override;

        bool hasCustomFont{ false };
        std::shared_ptr<Thsan::Font> customFont;
        std::shared_ptr<Thsan::Font> defaultFont;

        WidgetData widgetData;

        std::vector<std::shared_ptr<Widget>> childs;
        std::weak_ptr<Widget> parent;

        float elapsedTime{ 0.0f };
        std::string name;

        std::shared_ptr<Thsan::Text> text;
    };
};