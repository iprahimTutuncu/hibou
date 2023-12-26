#pragma once

#include <unordered_map>
#include <memory>
#include <thsan/graphics/texture.h>

namespace RessourceManager {

    class Texture2DManager
    {
    public:
        Texture2DManager();
        static std::shared_ptr<Thsan::Texture2D> get(const std::string& name);
        static void removeUnused();
        static const std::string default_texture_white;
        static const std::string default_texture_checkerboard;

        static const std::string default_texture_white;
        static const std::string default_texture_checkerboard;

    private:
        static std::unordered_map<std::string, std::shared_ptr<Thsan::Texture2D>> p_textures;
    };
}
