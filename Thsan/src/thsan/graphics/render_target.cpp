#include "pch.h"
#include "render_target.h"
#include "thsan/graphics/graphic_api.h"
#include "thsan/graphics/drawable.h"
#include "platform/openGL/gl_render_target.h"
#include "thsan/log.h"

namespace Thsan {

    std::shared_ptr<RenderTarget> create_renderTarget()
    {
        switch (get_graphic_API())
        {
            case GraphicAPI::openGL:
                return std::make_shared<GLRenderTarget>();
            case GraphicAPI::vulkan: return nullptr;
        };

        TS_CORE_ERROR("error: RenderTarget not supported for the current graphic API");
        return nullptr;
    }
    void RenderTarget::draw(const Mesh& mesh, RenderStates2D& states) const
    {
        states.bind();
        draw(mesh);
        states.unBind();
    }
    void RenderTarget::draw(const Drawable& drawable, RenderStates2D& states) const
    {
        drawable.draw(*this, states);
    }
}