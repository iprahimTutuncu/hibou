#include "pch.h"
#include "framebuffer.h"

#include "thsan/log.h"
#include "thsan/graphics/graphic_api.h"
#include "platform/openGL/gl_framebuffer.h"

namespace Thsan {

    std::shared_ptr<Framebuffer> Thsan::create_framebuffer(uint32_t width, uint32_t height)
    {
        bool size_defined = width != 0 && height != 0;
        switch (get_graphic_API())
        {
        case GraphicAPI::openGL:
            return size_defined ? std::make_shared<GLFramebuffer>(width, height): std::make_shared<GLFramebuffer>();
        case GraphicAPI::vulkan:
            return nullptr;
        };

        TS_CORE_ERROR("error: RenderTarget not supported for the current graphic API");
        return nullptr;
    }
    Framebuffer::Framebuffer(uint32_t width, uint32_t height):
        size(width, height)
    {
    }

    inline glm::uvec2 Framebuffer::getSize() const
    {
        return size;
    }

}
