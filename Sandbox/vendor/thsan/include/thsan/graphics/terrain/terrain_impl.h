#pragma once
#include "terrain.h"
#include <tsm/math/color.h>
#include <thsan/graphics/compute_shader.h>

namespace Thsan {
    class TerrainImpl : public Terrain {
    public:
        TerrainImpl() = default;
        virtual ~TerrainImpl() = default;

        // Implementation of the pure virtual functions
        virtual void init(TerrainOptions options, WindowOption windowOption) override;

        virtual float getRenderHeight(glm::vec2 position) override;
        virtual float getPhysicalHeight(glm::vec2 position) override;
        virtual float getCollisionMask(glm::vec2 position) override;
        virtual glm::vec3 getNormal(glm::vec2 position) override;
        virtual tsm::Color getAlbedo(glm::vec2 position) override;

        virtual void update(Thsan::TerrainOptions options, Thsan::WindowOption windowOption) override;

        virtual TerrainOptions getTerrainOptions() override;

        virtual std::shared_ptr<Texture2D> getRenderedDepthTexture() override;
        virtual std::shared_ptr<Texture2D> getRenderedAlbedoTexture() override;
        virtual std::shared_ptr<Texture2D> getRenderedPositionTexture() override;
        virtual std::shared_ptr<Texture2D> getRenderedNormalTexture() override;

    private:
        TerrainOptions options;

        std::vector<tsm::Color> albedoMap;
        std::vector<float> heightMap;
        std::vector<float> collisionHeightMap;
        std::vector<float> collisionMaskMap;
        std::vector<glm::vec3> normalMap;
        std::vector<float> ybuffer;

        std::shared_ptr<Texture2D> textureAlbedo;
        std::shared_ptr<Texture2D> textureHeight;
        std::shared_ptr<Texture2D> textureCollisionHeight;
        std::shared_ptr<Texture2D> textureCollisionMask;
        std::shared_ptr<Texture2D> textureNormal;

        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Texture2D> textureRenderedAlbedo;
        std::shared_ptr<Texture2D> textureRenderedDepth;
        std::shared_ptr<Texture2D> textureRenderedNormal;
        std::shared_ptr<Texture2D> textureRenderedPosition;
        std::shared_ptr<Texture2D> textureRenderedAO;

        std::shared_ptr<ComputeShader> terrainVisibilityCS;
    };
}