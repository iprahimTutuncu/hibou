#include "pch.h"
#include "texture.h"
#include "thsan/log.h"
#include "thsan/graphics/graphic_api.h"
#include "platform/openGL/gl_texture2D.h"

namespace Thsan {
    namespace textureImpl{
        int getUniqueId() {
            static std::atomic<uint32_t> uid{ 0 };
            return ++uid;
        }
    }

    Texture::Texture():
        id(textureImpl::getUniqueId()),
        smooth(false),
        repeated(false)
    {
    }

    bool Texture::operator==(const Texture& other)
    {
        return id == other.getID();
    }
	
    bool Texture::isSmooth() const
    {
        return smooth;
    }

    bool Texture::isRepeated() const
    {
        return repeated;
    }

    std::shared_ptr<Texture2D> create_texture2D()
    {
        switch (get_graphic_API())
        {
        case GraphicAPI::openGL:
            return std::make_shared<GLTexture2D>();
        case GraphicAPI::vulkan: return nullptr;
        };

        TS_CORE_ERROR("error: Texture2D not supported for the current graphic API");
        return nullptr;
    }
}
