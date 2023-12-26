#include "pch.h"
#include "texture2D_manager.h"

    RessourceManager::Texture2DManager::Texture2DManager()
    {
    }

    std::shared_ptr<Thsan::Texture2D> RessourceManager::Texture2DManager::get(const std::string& name)
    {
        const auto i = p_textures.find(name);
        if (i != p_textures.end())
            return i->second;
        else {
            std::shared_ptr<Thsan::Texture2D> p_texture;

            p_texture = Thsan::create_texture2D();


            if(name == default_texture_white)
            {
                const float white_pixels[] = {
                    1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 1.0f
                };
                Thsan::vec2u size{ 2, 2 };
                p_texture->loadFromMemory(white_pixels, size);
            }
            else if (name == default_texture_checkerboard)
            {
                float checkerboard_pixels[] = {
                    0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,
                    1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,
                    1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f,	1.f, 1.f, 1.f,		0.5f, 0.5f, 0.5f
                };

                Thsan::vec2u size{4, 4};
                p_texture->loadFromMemory(checkerboard_pixels, size);
            }
            else {
                p_texture->loadFromFile(name);
            }
            p_textures.insert({ name, p_texture });
            return p_texture;
        }
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
