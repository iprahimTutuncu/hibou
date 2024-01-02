#include "pch.h"
#include "texture2D_manager.h"
#include <thsan/graphics/texture.h>
#include <thsan/log.h>

void Thsan::RessourceManager::Texture2DManager::createDefaultWhiteTexture() {

    defaultWhite = Thsan::create_texture2D();

    glm::uvec2 size(2, 2);
    unsigned char* data = new unsigned char[size.x * size.y * 4];

    for (unsigned int i = 0; i < size.x * size.y * 4; i += 4) {
        data[i] = 255;
        data[i + 1] = 255;
        data[i + 2] = 255;
        data[i + 3] = 255;
    }

    if (!defaultWhite->create(size, Thsan::TextureFormat::RGBA8, data)) {
        TS_CORE_ERROR("Failed to create defaultWhiteTexture!");
    }

    delete[] data;
}

void Thsan::RessourceManager::Texture2DManager::createDefaultNormalTexture() {
    defaultNormal = Thsan::create_texture2D();

    glm::uvec2 size(2, 2);
    unsigned char* data = new unsigned char[size.x * size.y * 4];

    for (unsigned int i = 0; i < size.x * size.y * 4; i += 4) {
        data[i] = 128;
        data[i + 1] = 128;
        data[i + 2] = 255;
        data[i + 3] = 255;
    }

    if (!defaultNormal->create(size, Thsan::TextureFormat::RGBA8, data)) {
        TS_CORE_ERROR("Failed to create defaultNormalTexture!");
    }

    delete[] data;
}

std::shared_ptr<Thsan::Texture2D> Thsan::RessourceManager::Texture2DManager::get(const std::string& name)
{
    if (name == "default_white") {
        if (!defaultWhite)
            createDefaultWhiteTexture();
        return defaultWhite;
    }

    if (name == "default_normal") {
        if (!defaultNormal)
            createDefaultNormalTexture();
        return defaultNormal;
    }

    const auto i = p_textures.find(name);
    if (i != p_textures.end())
        return i->second;
    else {
        std::shared_ptr<Thsan::Texture2D> p_texture;

        p_texture = Thsan::create_texture2D();
        if (p_texture->loadFromFile(name)) {
            p_textures.insert({ name, p_texture });
        }
        else
            return nullptr;

        return p_texture;
    }
}

std::shared_ptr<Thsan::Texture2D> Thsan::RessourceManager::Texture2DManager::create(const std::string& name, RessourceManager::TextureDesc desc)
{
    auto& texture = p_textures[name];

    if (!texture) {
        texture = Thsan::create_texture2D();
        p_textures.insert({ name, texture });
    }

    texture->create(desc.size, desc.format, desc.data);

    return texture;
}


void Thsan::RessourceManager::Texture2DManager::removeUnused()
{
    for (auto i = p_textures.begin(); i != p_textures.end();)
        if (i->second.use_count() == 1)
            i = p_textures.erase(i);
        else
            ++i;
}

std::unordered_map<std::string, std::shared_ptr<Thsan::Texture2D>> Thsan::RessourceManager::Texture2DManager::p_textures;
std::shared_ptr<Thsan::Texture2D> Thsan::RessourceManager::Texture2DManager::defaultWhite;
std::shared_ptr<Thsan::Texture2D> Thsan::RessourceManager::Texture2DManager::defaultNormal;

