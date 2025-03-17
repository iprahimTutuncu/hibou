#pragma once

#include <unordered_map>
#include <memory>
#include <thsan/graphics/texture.h>

namespace RessourceManager
{
    struct TextureDesc 
    {
        glm::uvec2 size;
        Thsan::TextureFormat format;
        const void* data;
    };

    class Texture2DManager
    {
    public:
        Texture2DManager() = default;
        static std::shared_ptr<Thsan::Texture2D> get(const std::string& name, bool keepRaw = false, Thsan::TextureBitDepth bitDepth = Thsan::TextureBitDepth::BitDepth8);
        static std::shared_ptr<Thsan::Texture2D> create(const std::string& name, TextureDesc desc);
        static void removeUnused();
        static const std::string default_texture_white;
        static const std::string default_texture_checkerboard;

        static const std::string default_texture_white;
        static const std::string default_texture_checkerboard;

    private:
        static std::unordered_map<std::string, std::shared_ptr<Thsan::Texture2D>> p_textures;
        static constexpr float white_pixels[] = 
        {
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f
        };

        static constexpr float checkerboard_pixels[] = 
        {
           0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,
           1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,
           0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,
           1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f
        };

    };
}
