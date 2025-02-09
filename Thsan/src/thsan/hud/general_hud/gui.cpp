#include "pch.h"
#include "thsan/hud/general_hud/gui.h"
#include "thsan/hud/general_hud/widget.h"
#include "thsan/hud/general_hud/button.h"
#include <thsan/game.h>
#include <glm/vec2.hpp>
#include <thsan/log.h>
#include "thsan/hud/general_hud/label.h"
#include "thsan/hud/general_hud/label_button.h"

namespace Thsan::UI
{
    GuiImpl::GuiImpl()
        : parent(nullptr), pRenderer2D(nullptr), selection(nullptr)
    {
    }

    GuiImpl::~GuiImpl()
    {
        widgets.clear();
    }

    bool GuiImpl::handleEvent(const std::vector<Thsan::InputAction>& inputActions)
    {
        if (!selection)
            return false;

        static bool released = true;

        if (!inputActions.empty())
        {
            for (const auto& inputAction : inputActions)
            {
                if (inputAction == Thsan::InputAction::up)
                {
                    if (released)
                    {
                        if (selection->get(UI::Direction::up))
                        {
                            selection->call(UI::Action::crossed);
                            selection = selection->get(UI::Direction::up);
                        }
                        released = false;
                        return true;
                    }
                }
                else if (inputAction == Thsan::InputAction::down)
                {
                    if (released)
                    {
                        if (selection->get(UI::Direction::down))
                        {
                            selection->call(UI::Action::crossed);
                            selection = selection->get(UI::Direction::down);
                        }
                        released = false;
                        return true;
                    }
                }
                else if (inputAction == Thsan::InputAction::right)
                {
                    if (released)
                    {
                        if (selection->get(UI::Direction::right))
                        {
                            selection->call(UI::Action::crossed);
                            selection = selection->get(UI::Direction::right);
                        }
                        released = false;
                        return true;
                    }
                }
                else if (inputAction == Thsan::InputAction::left)
                {
                    if (released)
                    {
                        if (selection->get(UI::Direction::left))
                        {
                            selection->call(UI::Action::crossed);
                            selection = selection->get(UI::Direction::left);
                        }
                        released = false;
                        return true;
                    }
                }
                else if (inputAction == Thsan::InputAction::select)
                {
                    if (released)
                    {
                        selection->call(UI::Action::pressed);
                        released = false;
                        return true;
                    }
                }
                else
                {
                    released = true;
                }
            }
        }
        else
        {
            released = true;
        }

        if (selection->lastCall() != UI::Action::hover)
            selection->call(UI::Action::hover);

        return true;
    }

    void GuiImpl::update(float dt)
    {
        TS_CORE_ASSERT(screenWidth > 0 || screenHeight > 0, "In GuiImpl::update, screen size is not valid");

        for (auto& w : widgets)
        {
            auto& widgetData = w->getWidgetData();
            widgetData.xGlobal = 0;
            widgetData.yGlobal = 0;
            widgetData.wGlobal = screenWidth;
            widgetData.hGlobal = screenHeight;
            Widget::UpdateWidgetData(widgetData);

            w->update(dt);
        }
    }

    void GuiImpl::add(std::shared_ptr<Widget> widget)
    {
        widgets.push_back(widget);
        if (widget->getWidgetData().hasText)
        {
            if (widget->getWidgetData().isButton)
            {
                std::shared_ptr<LabelButton> labelBttn = std::static_pointer_cast<LabelButton>(widget);
                labelBttn->setFont(defaultFont);
            }
            else
            {
                std::shared_ptr<Label> label = std::static_pointer_cast<Label>(widget);
                label->setFont(defaultFont);
            }
        }
        if (widget->getWidgetData().isButton)
        {
            buttons.push_back(std::static_pointer_cast<Button>(widget));
        }
    }

    void GuiImpl::remove(const std::shared_ptr<Widget>& widget)
    {
        if (widget->getWidgetData().isButton)
        {
            auto it = std::find(buttons.begin(), buttons.end(), std::static_pointer_cast<Button>(widget));
            if (it != buttons.end())
                buttons.erase(it);
        }

        auto it = std::find(widgets.begin(), widgets.end(), widget);
        if (it != widgets.end())
        {
            widgets.erase(it);
        }
    }


    void GuiImpl::setScreenSize(int width, int height)
    {
        screenWidth = width;
        screenHeight = height;
    }

    void GuiImpl::setFont(std::shared_ptr<Thsan::Font> font)
    {
        defaultFont = font;

        for (auto& w : widgets) 
        {
            if (w->getWidgetData().hasText)
            {
                std::weak_ptr<Label> label = std::static_pointer_cast<Label>(w);
                if (auto labelPtr = label.lock())
                {
                    labelPtr->setFont(font);
                }
            }
        }
    }

    bool GuiImpl::setSelection(const std::shared_ptr<Button>& button)
    {
        if (std::find(buttons.begin(), buttons.end(), button) != buttons.end())
        {
            selection = button;
            return true;
        }
        return false;
    }

    std::shared_ptr<Button> GuiImpl::getSelection() const
    {
        return selection;
    }

    std::vector<std::shared_ptr<Widget>> GuiImpl::getWidgets() const
    {
        return widgets;
    }

    std::shared_ptr<Gui> Gui::create()
    {
        return std::make_shared<GuiImpl>();
    }
}