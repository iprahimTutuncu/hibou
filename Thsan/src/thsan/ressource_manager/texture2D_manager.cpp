#include "pch.h"
#include "thsan/ressource_manager/texture2D_manager.h"
#include "thsan/log.h"

    std::shared_ptr<Thsan::Texture2D> RessourceManager::Texture2DManager::get(const std::string& name, bool keepRaw, Thsan::TextureBitDepth bitDepth)
    {
        auto it = p_textures.find(name);
        if (it != p_textures.end())
            return it->second;

        std::shared_ptr<Thsan::Texture2D> p_texture = Thsan::Texture2D::create();

        if (name == default_texture_white)
            p_texture->generate({ 2, 2 }, Thsan::TextureFormat::RGB32F, white_pixels, false, true, false);
        else if (name == default_texture_checkerboard)
            p_texture->generate({ 4, 4 }, Thsan::TextureFormat::RGB32F, checkerboard_pixels, false, true, false);
        else if (!p_texture->loadFromFile(name, keepRaw, bitDepth)){
            auto default_it = p_textures.find(default_texture_checkerboard);
            if (default_it != p_textures.end())
                return default_it->second;

            // Generate checkerboard texture as default
            return get(default_texture_checkerboard, false);
        }

        p_textures.insert({ name, p_texture });
        return p_texture;
    }


    std::shared_ptr<Thsan::Texture2D> RessourceManager::Texture2DManager::create(const std::string& name, TextureDesc desc)
    {
        auto& texture = p_textures[name];

        if (!texture) {
            texture = Thsan::Texture2D::create();
            p_textures.insert({ name, texture });
        }

        TS_CORE_ASSERT(texture->generate(desc.size, desc.format, desc.data), "RessourceManager::Texture2DManager::create, cannot create texture!");

        return texture;
    }

    void RessourceManager::Texture2DManager::removeUnused()
    {
        for (auto i = p_textures.begin(); i != p_textures.end();)
            if (i->second.use_count() == 1)
                i = p_textures.erase(i);
            else
                ++i;
    }

    std::unordered_map<std::string, std::shared_ptr<Thsan::Texture2D>> RessourceManager::Texture2DManager::p_textures;

    const std::string RessourceManager::Texture2DManager::default_texture_white = "default_texture_white";
    const std::string RessourceManager::Texture2DManager::default_texture_checkerboard = "default_texture_checkerboard";
