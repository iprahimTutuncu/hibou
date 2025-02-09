#include "pch.h"
#include "thsan/hud/general_hud/panel.h"
#include "thsan/log.h"
#include "thsan/graphics/shader.h"
#include "thsan/graphics/mesh.h"
#include "thsan/ressource_manager/texture2D_manager.h"
#include "tsm/math/transform.h"

namespace Thsan::UI {
    PanelImpl::PanelImpl()
    {
        transform = tsm::Transform::create();
        mesh = Thsan::Mesh::create(4);

        (*mesh)[0].position = glm::vec3{ 0.f, 0.f, 0.f };
        (*mesh)[0].texCoord = glm::vec2{ 0.f, 0.f };
        (*mesh)[0].color = glm::vec4{ 1.f, 1.f, 1.f, 1.f };

        (*mesh)[1].position = glm::vec3{ 1.f, 0.f, 0.f };
        (*mesh)[1].texCoord = glm::vec2{ 1.0f, 0.f };
        (*mesh)[1].color = glm::vec4{ 1.f, 1.f, 1.f, 1.f };

        (*mesh)[2].position = glm::vec3{ 1.f, 1.f, 0.f };
        (*mesh)[2].texCoord = glm::vec2{ 1.0f, 1.f };
        (*mesh)[2].color = glm::vec4{ 1.f, 1.f, 1.f, 1.f };

        (*mesh)[3].position = glm::vec3{ 0.f, 1.f, 0.f };
        (*mesh)[3].texCoord = glm::vec2{ 0.f, 1.f };
        (*mesh)[3].color = glm::vec4{ 1.f, 1.f, 1.f, 1.f };

        mesh->setIndices({ 0, 1, 2, 0, 2, 3 });

        mesh->setPrimitiveType(Thsan::PrimitiveType::Triangles);
        mesh->generate();
    }

    void PanelImpl::setProperty(PanelProperty property, const PanelPropertyValue& value)
    {
        switch (property) {
            case PanelProperty::BorderColor1:
                shaderUIdata.colorBorder1 = std::get<tsm::Color>(value);
                break;
            case PanelProperty::BorderColor2:
                shaderUIdata.colorBorder2 = std::get<tsm::Color>(value);
                break;
            case PanelProperty::BorderColor3:
                shaderUIdata.colorBorder3 = std::get<tsm::Color>(value);
                break;
            case PanelProperty::BorderWidth:
                shaderUIdata.borderWidth = std::get<float>(value);
                break;
            case PanelProperty::CornerRadius:
                shaderUIdata.cornerRadius = std::get<float>(value);
                break;
            case PanelProperty::ShadowSize:
                shaderUIdata.shadowSize = std::get<float>(value);
                break;
            case PanelProperty::BgColor:
                shaderUIdata.backgroundColor = std::get<tsm::Color>(value);
                break;
            case PanelProperty::BgTexturePath:
                setBgTexture(std::get<std::string>(value));
                break;
            case PanelProperty::BgTextureEnabled:
                shaderUIdata.enableBgTexture = std::get<bool>(value);
                break;
            default:
                TS_CORE_ASSERT(false, "In PanelImpl::setProperty, Unknown PanelProperty.");
        }
    }

    void PanelImpl::setBgTexture(const std::string& path)
    {
        p_texture = RessourceManager::Texture2DManager::get(path);
    }

    std::shared_ptr<Thsan::Texture2D> PanelImpl::getBgTexture() const
    {
        return p_texture;
    }

    void PanelImpl::setShaderParams(const PanelData& param)
    {
        shaderUIdata = param;
    }

    PanelData PanelImpl::getShaderParams() const
    {
        return shaderUIdata;
    }

    void PanelImpl::update(float deltaTime)
    {
        transform->setTranslation(glm::vec3(widgetData.xGlobal, widgetData.yGlobal, 0.f));
        glm::vec3 scale(0.f);
        scale.x = widgetData.wLocal;
        scale.y = widgetData.hLocal;
        scale.z = 0.f;
        transform->setScale(scale);
    }

    void PanelImpl::remove(const std::shared_ptr<Widget>& widget)
    {
        auto it = std::remove(childs.begin(), childs.end(), widget);
        if (it != childs.end()) {
            childs.erase(it, childs.end());
        }
    }

    void PanelImpl::setMargin(glm::vec2 margin)
    {
        widgetData.marginX = margin.x;
        widgetData.marginY = margin.y;
    }

    void PanelImpl::setSize(glm::vec2 size)
    {
        widgetData.wLocal = size.x;
        widgetData.hLocal = size.y;
    }

    void PanelImpl::setAnchor(Anchor anchor)
    {
        widgetData.anchor = anchor;
    }

    glm::vec2 PanelImpl::getMargin() const
    {
        return glm::vec2(widgetData.marginX, widgetData.marginY);
    }

    glm::vec2 PanelImpl::getGlobalPosition() const
    {
        return glm::vec2(widgetData.xGlobal, widgetData.yGlobal);
    }

    glm::vec2 PanelImpl::getSize() const
    {
        return glm::vec2(widgetData.wLocal, widgetData.hLocal);
    }

    Anchor PanelImpl::getAnchor() const
    {
        return widgetData.anchor;
    }

    void PanelImpl::hide()
    {
        widgetData.hidden = true;
    }

    void PanelImpl::show()
    {
        widgetData.hidden = false;
    }

    bool PanelImpl::isHidden() const
    {
        return widgetData.hidden;
    }

    WidgetData& PanelImpl::getWidgetData()
    {
        return widgetData;
    }
    std::string PanelImpl::getName()
    {
        return name;
    }

    std::shared_ptr<Mesh> PanelImpl::getMesh() const
    {
        return mesh;
    }
    void PanelImpl::draw(const RenderTarget& target, RenderStates& states) const
    {
        // Get the shader from the render states
        auto s = states.getShader().lock();

        if (!s) {
            TS_CORE_ASSERT(s, "In PanelImpl::draw, shader is not available");
        }

        if (shaderUIdata.enableBgTexture && p_texture)
        {
            states.setTexture(TextureType::Default, p_texture);
        }

        // Set shader uniforms from shaderUIdata
        s->setVec4("borderColor1", shaderUIdata.colorBorder1.toGlm());
        s->setVec4("borderColor2", shaderUIdata.colorBorder2.toGlm());
        s->setVec4("borderColor3", shaderUIdata.colorBorder3.toGlm());
        s->setVec4("backgroundColor", shaderUIdata.backgroundColor.toGlm());
        s->setFloat("borderWidth", shaderUIdata.borderWidth);
        s->setFloat("cornerRadius", shaderUIdata.cornerRadius);
        s->setFloat("shadowSize", shaderUIdata.shadowSize);

        glm::vec2 panelSize = getSize();
        s->setVec2("panelSize", panelSize);
        s->setMat4("transform", transform->getTransform());
        s->setMat4("rotation", transform->getRotation());
        s->setInt("isText", false);
        s->setInt("hasBgTexture", shaderUIdata.enableBgTexture);

        // keyColor could be used for image with transparency and some effect
        // where a certain funny could be removed. Might be funny experiment.
        // all to be done in 2000 years from now. 
        s->setVec4("keyColor", tsm::Color(0.f, 0.f, 0.f, 0.f).toGlm());

        mesh->bind();
        target.draw(*mesh, states);
        mesh->unbind();

        s->setMat4("transform", glm::identity<glm::mat4>());
    }

    std::shared_ptr<Panel> Panel::create()
    {
        return std::make_shared<PanelImpl>();
    }
}