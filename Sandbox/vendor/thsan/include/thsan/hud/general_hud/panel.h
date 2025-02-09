#pragma once
#include <string>
#include <memory>
#include <vector>
#include <tsm/math/color.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "widget.h"
#include <variant>

enum class Type {
    none = 0,
    fill = 1
};

using PanelPropertyValue = std::variant<tsm::Color, float, glm::vec2, std::string, bool>;

namespace tsm
{
    class Transform;
}

namespace Thsan
{
    class Texture2D;
}


namespace Thsan::UI {

    struct THSAN_API PanelData
    {
        tsm::Color colorBorder1;
        tsm::Color colorBorder2;
        tsm::Color colorBorder3;
        tsm::Color backgroundColor;
        float borderWidth;
        float cornerRadius;
        float shadowSize;
        bool enableBgTexture;

        PanelData()
            : colorBorder1(tsm::Color(0, 0, 0, 0))
            , colorBorder2(tsm::Color(0, 0, 0, 0))
            , colorBorder3(tsm::Color(0, 0, 0, 0))
            , borderWidth(0.0f)
            , cornerRadius(0.0f)
            , shadowSize(0.0f)
        {
        }
    };

    enum class PanelProperty
    {
        BorderColor1,
        BorderColor2,
        BorderColor3,
        BorderWidth,
        CornerRadius,
        ShadowSize,
        BgColor,
        BgTexturePath,
        BgTextureEnabled,
        TransparencyEnabled
    };


    class THSAN_API Panel : public Widget
    {
    public:
        virtual ~Panel() = default;

        virtual void setProperty(PanelProperty property, const PanelPropertyValue& value) = 0;

        // Background texture management
        virtual void setBgTexture(const std::string& path) = 0;
        virtual std::shared_ptr<Thsan::Texture2D> getBgTexture() const = 0;

        // Shader parameter management
        virtual void setShaderParams(const PanelData& param) = 0;
        virtual PanelData getShaderParams() const = 0;

        static std::shared_ptr<Panel> create();
    };

    class PanelImpl : public Panel
    {
    public:
        PanelImpl();
        ~PanelImpl() = default;
        //inherited from Panel
        void setProperty(PanelProperty property, const PanelPropertyValue& value) override;

        void setBgTexture(const std::string& path) override;
        std::shared_ptr<Thsan::Texture2D> getBgTexture() const override;

        void setShaderParams(const PanelData& param) override;
        PanelData getShaderParams() const override;

        //inherited from Widget
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


    private:
        void draw(const RenderTarget& target, RenderStates& states) const override;


        std::weak_ptr<Widget> parent;
        std::vector<std::shared_ptr<Widget>> childs;

        float elapsedTime{ 0.0f };

        std::string name;
        WidgetData widgetData;
        PanelData shaderUIdata;
        std::shared_ptr<Thsan::Mesh> mesh;
        std::shared_ptr<tsm::Transform> transform;

        std::shared_ptr<Thsan::Texture2D> p_texture;
    };
}
