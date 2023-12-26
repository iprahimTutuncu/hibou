#include "texture2D_manager.h"
#include <iostream>


void RessourceManager::Texture2DManager::createDefaultWhiteTexture() {

    defaultWhite = std::make_shared<Thsan::Texture2D>();

    glm::uvec2 size(2, 2);
    unsigned char* data = new unsigned char[size.x * size.y * 4];

    for (int i = 0; i < size.x * size.y * 4; i += 4) {
        data[i] = 255;
        data[i + 1] = 255;
        data[i + 2] = 255;
        data[i + 3] = 255;
    }

    if (!defaultWhite->create(size, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, data)) {
        std::cerr << "Failed to create defaultWhiteTexture!" << std::endl;
    }

    delete[] data;
}

void RessourceManager::Texture2DManager::createDefaultNormalTexture() {
    defaultNormal = std::make_shared<Thsan::Texture2D>();

    glm::uvec2 size(2, 2);
    unsigned char* data = new unsigned char[size.x * size.y * 4];

    for (int i = 0; i < size.x * size.y * 4; i += 4) {
        data[i] = 128;
        data[i + 1] = 128;
        data[i + 2] = 255;
        data[i + 3] = 255;
    }

    if (!defaultNormal->create(size, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, data)) {
        std::cerr << "Failed to create defaultNormalTexture!" << std::endl;
    }

    delete[] data;
}

std::shared_ptr<Thsan::Texture2D> RessourceManager::Texture2DManager::get(const std::string& name)
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

        p_texture = std::make_shared<Thsan::Texture2D>();
        if (p_texture->loadFromFile(name)) {
            p_textures.insert({ name, p_texture });
        }
        else
            return nullptr;

        return p_texture;
    }
}

std::shared_ptr<Thsan::Texture2D> RessourceManager::Texture2DManager::create(const std::string& name, RessourceManager::TextureDesc desc)
{
    auto& texture = p_textures[name];

    if (!texture) {
        texture = std::make_shared<Thsan::Texture2D>();
        p_textures.insert({ name, texture });
    }

    texture->create(desc.size, desc.internalFormat, desc.format, desc.type, desc.data);

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
std::shared_ptr<Thsan::Texture2D> RessourceManager::Texture2DManager::defaultWhite;
std::shared_ptr<Thsan::Texture2D> RessourceManager::Texture2DManager::defaultNormal;

