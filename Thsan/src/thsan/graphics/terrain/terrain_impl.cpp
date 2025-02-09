#include "pch.h"
#include "thsan/graphics/terrain/terrain_impl.h"
#include "thsan/graphics/texture.h"
#include "thsan/graphics/shader.h"
#include "thsan/ressource_manager/texture2D_manager.h"

namespace Thsan {
    void TerrainImpl::init(TerrainOptions options, WindowOption windowOption)
    {
        terrainVisibilityCS = Thsan::ComputeShader::create("media/compute_shader/terrainVisibility.cs");

        this->options = options;
        this->textureAlbedo = RessourceManager::Texture2DManager::get(options.albedoTexturePath, true);
        this->textureHeight = RessourceManager::Texture2DManager::get(options.heightTexturePath, true);
        this->textureCollisionHeight = RessourceManager::Texture2DManager::get(options.collisionHeightTexturePath, true);
        this->textureCollisionMask = RessourceManager::Texture2DManager::get(options.collisionMaskTexturePath, true);
  
        // Albedo terrain informaton
        const float* rawData = this->textureAlbedo->getRawData();

        int width = this->textureAlbedo->getWidth();
        int height = this->textureAlbedo->getHeight();
        int numChannels = this->textureAlbedo->getNumChannels();

        // Loop through the RGBA data and convert it to tsm::Color
        for (int i = 0; i < width * height * numChannels; i += numChannels) {
            int r = static_cast<int>(256.f * rawData[i]);
            int g = static_cast<int>(256.f * rawData[i + 1]);
            int b = static_cast<int>(256.f * rawData[i + 2]);
            int a = numChannels > 3 ? static_cast<int>(256.f * rawData[i + 3]) : 0;

            tsm::Color color(r, g, b, a);
            albedoMap.push_back(color);
        }

        // Loop through height
        const float* rawHeightData = this->textureHeight->getRawData();
        const float* rawCollisionHeightData = this->textureCollisionHeight->getRawData();

        width = this->textureHeight->getWidth();
        height = this->textureHeight->getHeight();
        numChannels = this->textureHeight->getNumChannels();

        // Loop through the RGBA data and convert it to tsm::Color
        for (int i = 0; i < width * height * numChannels; i += numChannels)
            heightMap.push_back(rawHeightData[i]);
        
        for (int i = 0; i < width * height * numChannels; i += numChannels)
            collisionHeightMap.push_back(rawCollisionHeightData[i]);

        textureRenderedAlbedo = RessourceManager::Texture2DManager::create("textureRenderedAlbedo", { glm::uvec2(windowOption.screenWidth, windowOption.screenHeight), TextureFormat::RGBA8, nullptr });
        textureRenderedDepth = RessourceManager::Texture2DManager::create("textureRenderedDepth", { glm::uvec2(windowOption.screenWidth, windowOption.screenHeight), TextureFormat::R32F, nullptr });
        textureRenderedPosition = RessourceManager::Texture2DManager::create("textureRenderedPosition", { glm::uvec2(windowOption.screenWidth, windowOption.screenHeight), TextureFormat::RGBA32F, nullptr });
        textureRenderedNormal = RessourceManager::Texture2DManager::create("textureRenderedNormal", { glm::uvec2(windowOption.screenWidth, windowOption.screenHeight), TextureFormat::RGBA32F, nullptr });
    }

    float TerrainImpl::getRenderHeight(glm::vec2 position)
    {
        // On get le width et le height de la texture
        int32_t width = static_cast<int32_t>(textureHeight->getWidth());
        int32_t height = static_cast<int32_t>(textureHeight->getHeight());

        // Convertir en coord de texture
        int32_t x = static_cast<int32_t>(position.x + width / 2);
        int32_t y = static_cast<int32_t>(height / 2 + position.y); // Inverser le y

        if (x < 0 || x >= width || y < 0 || y >= height)
        {
            return 0.0f; // defaut à 0 ca semble socialement acceptable
        }

        int32_t index = y * width + x;
        return heightMap[index];
    }

    float TerrainImpl::getPhysicalHeight(glm::vec2 position)
    {
        // On get le width et le height de la texture
        int32_t width = static_cast<int32_t>(textureCollisionHeight->getWidth());
        int32_t height = static_cast<int32_t>(textureCollisionHeight->getHeight());

        // Convertir en coord de texture
        int32_t x = static_cast<int32_t>(position.x + width / 2);
        int32_t y = static_cast<int32_t>(height / 2 + position.y); // Inverser le y

        if (x < 0 || x >= width || y < 0 || y >= height)
        {
            return 0.0f; // defaut à 0 ca semble socialement acceptable
        }

        int32_t index = y * width + x;
        return collisionHeightMap[index] * options.scaleHeight;
    }

    float TerrainImpl::getCollisionMask(glm::vec2 position) 
    {
        // pImplement the logic to get collision mask at the specified position
        return 0.0f;
    }

    glm::vec3 TerrainImpl::getNormal(glm::vec2 position)
    {
        // On get le width et le height de la texture
        int32_t width = static_cast<int32_t>(textureCollisionHeight->getWidth());
        int32_t height = static_cast<int32_t>(textureCollisionHeight->getHeight());

        // Convertir en coord de texture
        int32_t x = static_cast<int32_t>(position.x + width / 2);
        int32_t y = static_cast<int32_t>(height / 2 + position.y); // Inverser le y

        if (x < 0 || x >= width || y < 0 || y >= height)
        {
            TS_CORE_WARN("TerrainImpl::getNormal(glm::vec2 position), out of boud, so je retourne vecteur qui va vers le haut.");
            return glm::vec3(0.f, 1.f, 0.f);
        }
        int32_t index = y * width + x;

        int32_t offset = 1;

        // Calculate the indices for neighboring pixels
        int32_t indexL = (y * width + (x - offset)); // Left
        int32_t indexR = (y * width + (x + offset)); // Right
        int32_t indexD = ((y + offset) * width + x); // Down
        int32_t indexU = ((y - offset) * width + x); // Up

        float hL = collisionHeightMap[indexL] / 255.0f;
        float hR = collisionHeightMap[indexR] / 255.0f;
        float hD = collisionHeightMap[indexD] / 255.0f;
        float hU = collisionHeightMap[indexU] / 255.0f;

        glm::vec3 nTerrain;
        nTerrain.x = (hL - hR);
        nTerrain.y = 2.0f / this->options.scaleHeight; // 2 * (1/scale_height) * (256/scaleHeight)
        nTerrain.z = (hU - hD);

        nTerrain = glm::normalize(nTerrain);

        return nTerrain;
    }

    tsm::Color TerrainImpl::getAlbedo(glm::vec2 position)
    {
        // On get le width et le height de la texture
        int32_t width = static_cast<int32_t>(textureAlbedo->getWidth());
        int32_t height = static_cast<int32_t>(textureAlbedo->getHeight());

        // Convertir en coord de texture
        int32_t x = static_cast<int32_t>(position.x + width / 2);
        int32_t y = static_cast<int32_t>(height / 2 + position.y); // Inverser le y

        if (x < 0 || x >= width || y < 0 || y >= height)
        {
            return tsm::Color(); // defaut à 0 ca semble socialement acceptable
        }

        int32_t index = y * width + x;
        return albedoMap[index];
    }

    void TerrainImpl::update(Thsan::TerrainOptions data, Thsan::WindowOption windowOption)
    {
        options = data;

        textureRenderedAlbedo->clear(0.f, 0.f, 0.f, 0.f);
        textureRenderedDepth->clear(1.f, 1.f, 1.f, 1.f);
        textureRenderedPosition->clear(0.f, 0.f, 0.f, 0.f);
        textureRenderedNormal->clear(0.f, 0.f, 0.f, 0.f);

        terrainVisibilityCS->use();

        terrainVisibilityCS->setVec3("cameraPosition", glm::vec3(options.position[0], options.position[1], options.position[2]));
        terrainVisibilityCS->setVec2("normalBlurr", glm::vec2(options.normalBlurr[0], options.normalBlurr[1]));
        terrainVisibilityCS->setFloat("screenWidth", windowOption.screenWidth);
        terrainVisibilityCS->setFloat("screenHeight", windowOption.screenHeight);
        terrainVisibilityCS->setInt("terrainSize", options.terrainSize);
        terrainVisibilityCS->setInt("normalBlurIteration", options.normalBlurIteration);
        terrainVisibilityCS->setFloat("LOD", options.LOD);
        terrainVisibilityCS->setFloat("LODStartAt", options.LODStartAt);
        terrainVisibilityCS->setFloat("angle", options.angle);
        terrainVisibilityCS->setFloat("horizon", options.horizon);
        terrainVisibilityCS->setFloat("scaleHeight", options.scaleHeight);
        terrainVisibilityCS->setFloat("spriteRenderDistance", options.spriteRenderDistance);
        terrainVisibilityCS->setFloat("terrainRenderDistance", options.terrainRenderDistance);
        terrainVisibilityCS->setFloat("shininess", options.shininess);
        terrainVisibilityCS->setInt("blurType", options.blurType);

        terrainVisibilityCS->setRWImage2D(textureRenderedAlbedo, 0);
        terrainVisibilityCS->setRWImage2D(textureRenderedDepth, 1);
        terrainVisibilityCS->setRWImage2D(textureRenderedPosition, 2);
        terrainVisibilityCS->setRWImage2D(textureRenderedNormal, 3);
        terrainVisibilityCS->setImage2D(textureAlbedo, 4);
        terrainVisibilityCS->setImage2D(textureHeight, 5);

        unsigned int screenWidth = static_cast<unsigned int>(windowOption.screenWidth);
        unsigned int screenHeight = static_cast<unsigned int>(windowOption.screenHeight);

        // Workgroup size (32x32)
        unsigned int workgroupSizeX = 32*32;
        unsigned int workgroupSizeY = 1;

        // Calculate number of workgroups needed for the X and Y directions
        unsigned int dispatchX = (screenWidth + workgroupSizeX - 1) / workgroupSizeX;
        unsigned int dispatchY = 1;

        unsigned int dispatchZ = 1;

        terrainVisibilityCS->dispatch(dispatchX, dispatchY, dispatchZ);
        terrainVisibilityCS->wait();

    }

    TerrainOptions TerrainImpl::getTerrainOptions()
    {
        return options;
    }

    std::shared_ptr<Texture2D> TerrainImpl::getRenderedDepthTexture()
    {
        return textureRenderedDepth;
    }

    std::shared_ptr<Texture2D> TerrainImpl::getRenderedAlbedoTexture()
    {
        return textureRenderedAlbedo;
    }

    std::shared_ptr<Texture2D> TerrainImpl::getRenderedPositionTexture()
    {
        return textureRenderedPosition;
    }
    std::shared_ptr<Texture2D> TerrainImpl::getRenderedNormalTexture()
    {
        return textureRenderedNormal;
    }
}
