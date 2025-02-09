#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include <thsan/graphics/drawable.h>

namespace Thsan::UI {

    enum class Anchor {
        upperLeft,
        upperRight,
        bottomLeft,
        bottomRight
    };

    struct THSAN_API WidgetData {
        bool hidden{ false };
        bool longTextEnable{ false };
        bool isButton{ false };
        bool hasText{ false };
        bool hasUnregisteredButton{ false };

        Anchor anchor{ Anchor::upperLeft };

        int wLocal, hLocal;
        int marginX, marginY;

        int xGlobal, yGlobal;
        int wGlobal, hGlobal;
    };

    class THSAN_API Widget : public Thsan::Drawable
    {
    public:
        Widget() = default;
        virtual ~Widget() = default;

        // Pure virtual functions
        virtual void update(float deltaTime) = 0;

        virtual void remove(const std::shared_ptr<Widget>& widget) = 0;

        virtual void setMargin(glm::vec2 margin) = 0;
        virtual void setSize(glm::vec2 size) = 0;
        virtual void setAnchor(Anchor anchor) = 0;

        virtual glm::vec2 getMargin() const = 0;
        virtual glm::vec2 getGlobalPosition() const = 0;
        virtual glm::vec2 getSize() const = 0;
        virtual Anchor getAnchor() const = 0;

        virtual void hide() = 0;
        virtual void show() = 0;
        virtual bool isHidden() const = 0;

        virtual WidgetData& getWidgetData() = 0;

        virtual std::string getName() = 0;

        static void UpdateWidgetData(WidgetData& widget)
        {
            switch (widget.anchor) {
            case Anchor::upperLeft:
                widget.xGlobal += widget.marginX;
                widget.yGlobal += widget.marginY;
                break;
            case Anchor::upperRight:
                widget.xGlobal += (widget.wGlobal - widget.wLocal) - widget.marginX;
                widget.yGlobal += widget.marginY;
                break;
            case Anchor::bottomLeft:
                widget.xGlobal += widget.marginX;
                widget.yGlobal += (widget.hGlobal - widget.hLocal) - widget.marginY;
                break;
            case Anchor::bottomRight:
                widget.xGlobal += (widget.wGlobal - widget.wLocal) - widget.marginX;
                widget.yGlobal += (widget.hGlobal - widget.hLocal) - widget.marginY;
                break;
            }

            widget.wGlobal = widget.wLocal;
            widget.hGlobal = widget.hLocal;
        }

    protected:
        virtual void draw(const RenderTarget& target, RenderStates& states) const override = 0;

        friend class Gui;
    };
}
