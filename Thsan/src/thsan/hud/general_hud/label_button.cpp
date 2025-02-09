#include "pch.h"
#include "thsan/hud/general_hud/label_button.h"
#include <thsan/graphics/shader.h>

namespace Thsan::UI {
    LabelButtonImpl::LabelButtonImpl(std::string name):
        name(name)
    {
        text = Thsan::Text::create();
        this->widgetData.hasText = true;
        this->widgetData.isButton = true;
    }

    // Implement abstract methods from Label
    void LabelButtonImpl::setString(const std::u32string& string)
    {
        text->setString(string);
    }

    const std::u32string LabelButtonImpl::getString() const
    {
        return text->getString();
    }

    void LabelButtonImpl::enableCustomFont()
    {
        hasCustomFont = true;
    }

    void LabelButtonImpl::disableCustomFont()
    {
        hasCustomFont = false;
    }

    void LabelButtonImpl::draw(const RenderTarget& target, RenderStates& states) const
    {
        if(auto shader = states.getShader().lock())
        {
            shader->setInt("hasBgTexture", true);
            shader->setInt("isText", true);
        }

        target.draw(*text, states);
    }

    void LabelButtonImpl::setFont(std::shared_ptr<Thsan::Font> font)
    {
        defaultFont = font;

        if (hasCustomFont)
            return;

        text->setFont(font);
        text->setString(text->getString());
    }

    void LabelButtonImpl::setCustomFont(std::shared_ptr<Thsan::Font> font)
    {
        customFont = font;
        text->setFont(font);
        text->setString(text->getString());
    }

    void LabelButtonImpl::setColor(tsm::Color color)
    {
        text->setColor(color);
        text->setString(text->getString());
    }

    tsm::Color LabelButtonImpl::getColor() const
    {
        return text->getColor();
    }

    // Implement abstract methods from Button
    void LabelButtonImpl::connect(Action action, std::function<void()> functor)
    {
        actions[action] = std::move(functor);
    }

    void LabelButtonImpl::call(Action action)
    {
        auto it = actions.find(action);
        if (it != actions.end()) {
            it->second();
            lastAction = action;
        }
    }

    Action LabelButtonImpl::lastCall() const
    {
        return lastAction;
    }

    void LabelButtonImpl::set(Direction direction, const std::shared_ptr<Button>& button)
    {
        directionalButtons[direction] = button;
    }

    std::shared_ptr<Button> LabelButtonImpl::get(Direction direction) const
    {
        auto it = directionalButtons.find(direction);
        if (it != directionalButtons.end()) {
            return it->second.lock();
        }
        return nullptr;
    }

    // Implement abstract methods from Widget
    void LabelButtonImpl::update(float deltaTime)
    {
        elapsedTime += deltaTime;

        text->setPosition(glm::vec3(widgetData.xGlobal, widgetData.yGlobal, 0.f));
        glm::vec3 scale(0.f);
        scale.x = widgetData.wLocal / text->getTextWidth();
        scale.y = widgetData.hLocal / text->getTextHeight();
        scale.z = 0.f;
        text->setScale(scale);
    }

    void LabelButtonImpl::remove(const std::shared_ptr<Widget>& widget)
    {
        auto it = std::remove(childs.begin(), childs.end(), widget);
        if (it != childs.end()) {
            childs.erase(it, childs.end());
        }
    }

    void LabelButtonImpl::setSizeToFontSize()
    {
        widgetData.wLocal = text->getTextWidth();
        widgetData.hLocal = text->getTextHeight();
    }

    glm::ivec2 LabelButtonImpl::getFontSize()
    {
        return glm::ivec2(text->getTextWidth(), text->getTextHeight());
    }

    void LabelButtonImpl::setMargin(glm::vec2 margin)
    {
        widgetData.marginX = margin.x;
        widgetData.marginY = margin.y;
    }

    void LabelButtonImpl::setSize(glm::vec2 size)
    {
        widgetData.wLocal = size.x;
        widgetData.hLocal = size.y;
    }

    void LabelButtonImpl::setAnchor(Anchor anchor)
    {
        widgetData.anchor = anchor;
    }

    glm::vec2 LabelButtonImpl::getMargin() const
    {
        return glm::vec2(widgetData.marginX, widgetData.marginY);
    }

    glm::vec2 LabelButtonImpl::getGlobalPosition() const
    {
        return glm::vec2(widgetData.xGlobal, widgetData.yGlobal);
    }

    glm::vec2 LabelButtonImpl::getSize() const
    {
        return glm::vec2(widgetData.wLocal, widgetData.hLocal);
    }

    Anchor LabelButtonImpl::getAnchor() const
    {
        return widgetData.anchor;
    }

    void LabelButtonImpl::hide()
    {
        widgetData.hidden = true;
    }

    void LabelButtonImpl::show()
    {
        widgetData.hidden = false;
    }

    bool LabelButtonImpl::isHidden() const
    {
        return widgetData.hidden;
    }

    WidgetData& LabelButtonImpl::getWidgetData()
    {
        return widgetData;
    }

    std::string LabelButtonImpl::getName()
    {
        return name;
    }

    std::shared_ptr<Mesh> LabelButtonImpl::getMesh() const
    {
        return text->getMesh();
    }

    std::shared_ptr<LabelButton> LabelButton::create(std::string name)
    {
        return std::make_shared<LabelButtonImpl>(name);
    }

} // namespace Thsan::UI
