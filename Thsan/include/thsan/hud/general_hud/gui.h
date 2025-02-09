#pragma once

#include <vector>
#include <memory>
#include <thsan/Input/control_setting.h>
#include "widget.h"

namespace Thsan {
    class Renderer2D;
    class Game;
    class Font;
}

namespace Thsan::UI
{
    class Button;

    class THSAN_API Gui
    {
    public:
        Gui() = default;
        ~Gui() = default;

        virtual bool handleEvent(const std::vector<Thsan::InputAction>& inputActions) = 0;
        virtual void update(float dt) = 0;

        virtual void add(std::shared_ptr<Widget> widget) = 0;

        // Remove will also remove any child widgets inside
        virtual void remove(const std::shared_ptr<Widget>& widget) = 0;

        virtual void setScreenSize(int width, int height) = 0;
        virtual void setFont(std::shared_ptr<Thsan::Font> font) = 0;
        virtual bool setSelection(const std::shared_ptr<Button>& button) = 0; // O(n)
        virtual std::shared_ptr<Button> getSelection() const = 0;

        virtual std::vector<std::shared_ptr<Widget>> getWidgets() const = 0;

        static std::shared_ptr<Gui> create();
    };

    class GuiImpl : public Gui
    {
    public:
        GuiImpl();
        ~GuiImpl();

        bool handleEvent(const std::vector<Thsan::InputAction>& inputActions) override;
        void update(float dt) override;

        void add(std::shared_ptr<Widget> widget) override;

        // Remove will also remove any child widgets inside
        void remove(const std::shared_ptr<Widget>& widget) override;

        void setScreenSize(int width, int height) override;
        void setFont(std::shared_ptr<Thsan::Font> font) override;

        bool setSelection(const std::shared_ptr<Button>& button) override; // O(n)
        std::shared_ptr<Button> getSelection() const override;

        std::vector<std::shared_ptr<Widget>> getWidgets() const override;

    private:
        Thsan::Game* parent;
        Thsan::Renderer2D* pRenderer2D; // This will be given by Game class
        std::vector<std::shared_ptr<Widget>> widgets;
        std::vector<std::shared_ptr<Button>> buttons;
        std::shared_ptr<Button> selection;
        std::shared_ptr<Thsan::Font> defaultFont;

        int screenWidth{ -1 };
        int screenHeight{ -1 };
    };
}
