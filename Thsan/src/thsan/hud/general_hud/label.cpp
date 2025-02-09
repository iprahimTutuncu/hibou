#include "pch.h"
#include "thsan/hud/general_hud/label.h"
#include <thsan/graphics/drawable/text.h>
#include <tsm/math/color.h>
#include <thsan/graphics/font.h>
#include <thsan/graphics/shader.h>

namespace Thsan::UI {

    std::shared_ptr<Label> Thsan::UI::Label::create(std::string name)
    {
        return std::make_shared<LabelImpl>(name);
    }

    LabelImpl::LabelImpl(std::string name)
    {
        text = Thsan::Text::create();
        widgetData.hasText = true;
    }

    void LabelImpl::setString(const std::u32string& text)
    {
        this->text->setString(text);
    }

    const std::u32string LabelImpl::getString() const
    {
        return text->getString();
    }

    void LabelImpl::enableCustomFont()
    {
        hasCustomFont = true;
    }

    void LabelImpl::disableCustomFont()
    {
        hasCustomFont = false;
    }

    void LabelImpl::setSizeToFontSize()
    {
        widgetData.wLocal = text->getTextWidth();
        widgetData.hLocal = text->getTextHeight();
    }

    glm::ivec2 LabelImpl::getFontSize()
    {
        return glm::ivec2(text->getTextWidth(), text->getTextHeight());
    }

    void LabelImpl::draw(const RenderTarget& target, RenderStates& states) const
    {   
        if (auto shader = states.getShader().lock())
        {
            shader->setInt("hasBgTexture", true);
            shader->setInt("isText", true);
        }
        target.draw(*text, states);
    }

    void LabelImpl::setFont(std::shared_ptr<Thsan::Font> font)
    {
        defaultFont = font;

        if (hasCustomFont)
            return;

        text->setFont(font);
        text->setString(text->getString());
    }

    void LabelImpl::setCustomFont(std::shared_ptr<Thsan::Font> font)
    {
        customFont = font;
        text->setFont(font);
        text->setString(text->getString());
    }

    void LabelImpl::setColor(tsm::Color color)
    {
        text->setColor(color);
        text->setString(text->getString());
    }

    tsm::Color LabelImpl::getColor() const
    {
        return text->getColor();
    }

    void LabelImpl::update(float deltaTime)
    {
        elapsedTime += deltaTime;

        text->setPosition(glm::vec3(widgetData.xGlobal, widgetData.yGlobal, 0.f));
        glm::vec3 scale(0.f);
        scale.x = widgetData.wLocal / text->getTextWidth();
        scale.y = widgetData.hLocal / text->getTextHeight();
        scale.z = 0.f;
        text->setScale(scale);
    }

    void LabelImpl::remove(const std::shared_ptr<Widget>& widget)
    {
        auto it = std::remove(childs.begin(), childs.end(), widget);
        if (it != childs.end()) {
            childs.erase(it, childs.end());
        }
    }

    void LabelImpl::setMargin(glm::vec2 margin)
    {
        widgetData.marginX = margin.x;
        widgetData.marginY = margin.y;
    }

    void LabelImpl::setSize(glm::vec2 size)
    {
        widgetData.wLocal = size.x;
        widgetData.hLocal = size.y;
    }

    void LabelImpl::setAnchor(Anchor anchor)
    {
        widgetData.anchor = anchor;
    }

    glm::vec2 LabelImpl::getMargin() const
    {
        return glm::vec2(widgetData.marginX, widgetData.marginY);
    }

    glm::vec2 LabelImpl::getGlobalPosition() const
    {
        return glm::vec2(widgetData.xGlobal, widgetData.yGlobal);
    }

    glm::vec2 LabelImpl::getSize() const
    {
        return glm::vec2(widgetData.wLocal, widgetData.hLocal);
    }

    Anchor LabelImpl::getAnchor() const
    {
        return widgetData.anchor;
    }

    void LabelImpl::hide()
    {
        widgetData.hidden = true;
    }

    void LabelImpl::show()
    {
        widgetData.hidden = false;
    }

    bool LabelImpl::isHidden() const
    {
        return widgetData.hidden;
    }

    WidgetData& LabelImpl::getWidgetData()
    {
        return widgetData;
    }
    std::string LabelImpl::getName()
    {
        return name;
    }

    std::shared_ptr<Mesh> LabelImpl::getMesh() const
    {
        return text->getMesh();
    }
}