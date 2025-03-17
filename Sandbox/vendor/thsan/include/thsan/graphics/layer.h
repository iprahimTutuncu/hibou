#pragma once
#include "thsan/core.h"
#include "thsan/graphics/texture.h"
#include <memory>
#include <vector>
#include <array>

namespace Thsan
{
    enum class LayerTextureType
    {
        Mask,
        Diffuse,
        Depth,
        ModifiedDepth,
        Position,
        Normal,
    };

    class THSAN_API Layer
    {
    public:
        virtual ~Layer() = default;

        static std::shared_ptr<Layer> create();

        virtual std::shared_ptr<Texture2D> getTexture(LayerTextureType type, size_t index = 0) const = 0;
        virtual void setTexture(LayerTextureType type, std::shared_ptr<Texture2D> texture, size_t index = 0) = 0;

        virtual float getDepthValue() const = 0;
        virtual void setDepthValue(float value) = 0;

        virtual std::string getName() const = 0;
        virtual void setName(std::string name) = 0;

        virtual size_t getMaximumIndex() = 0;
        virtual void setMaximumIndex(size_t index) = 0;

        float fx = 1.0f, fy = 1.0f, cx = 0.0f, cy = 0.0f;
        float k1 = 0.0f, k2 = 0.0f;

        float depthPrecision = 1.0f;
        float depthClampLowerBound = 1.0f;
        float depthClampUpperBound = 1.0f;
        int depthBlurKernelSize = 5;
        float depthblurSigma = 0.1f;
        float depthBlurableLimitDistance = 0.1f;
    };


    class LayerImpl : public Layer
    {
    public:
        LayerImpl();

        virtual std::shared_ptr<Texture2D> getTexture(LayerTextureType type, size_t index = 0) const override;
        virtual void setTexture(LayerTextureType type, std::shared_ptr<Texture2D> texture, size_t index = 0) override;

        virtual float getDepthValue() const override;
        virtual void setDepthValue(float value) override;

        virtual std::string getName() const override;
        virtual void setName(std::string name) override;

        virtual size_t getMaximumIndex() override;
        virtual void setMaximumIndex(size_t index) override;

    private:
        static const size_t max_texture_type_count = 6;
        std::array<std::vector<std::shared_ptr<Texture2D>>, max_texture_type_count> textures; // Mask, Diffuse, Depth, ModifiedDepth, position, normal. Ils ont tous la même quantité d'element. peuvent être nullptr
        float depth_value;
        std::string name;

        static std::vector<std::string> existing_name;
    };
}