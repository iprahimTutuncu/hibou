#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <thsan/graphics/texture.h>
#include <glm/ext/vector_uint2.hpp>

namespace RessourceManager {

    struct TextureDesc {
        glm::uvec2& size;
        Thsan::TextureFormat format;
        const void* data;
    };

    class Texture2DManager
    {
    public:
        Texture2DManager() = delete;
        static std::shared_ptr<Thsan::Texture2D> get(const std::string& name);
        static [[nodiscard]] std::shared_ptr<Thsan::Texture2D> getNoCreate(const std::string& name);
        static std::shared_ptr<Thsan::Texture2D> create(const std::string& name, TextureDesc desc);
        static void removeUnused();

    private:
        static void createDefaultWhiteTexture();
        static void createDefaultNormalTexture();
        static std::unordered_map<std::string, std::shared_ptr<Thsan::Texture2D>> p_textures;
        static std::shared_ptr<Thsan::Texture2D> defaultWhite;
        static std::shared_ptr<Thsan::Texture2D> defaultNormal;
    };
}
