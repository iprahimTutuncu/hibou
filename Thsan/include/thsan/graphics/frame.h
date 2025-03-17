#pragma once
#include "thsan/graphics/texture.h"
#include "thsan/graphics/light.h"
#include <memory>

namespace Thsan
{
    const int max_layer = 4;
    const int max_light = 16;

    enum class FrameTextureType
    {
        RayTracedResult
    };

    class THSAN_API Frame
    {
    public:
        virtual ~Frame() = default;

        static std::shared_ptr<Frame> create(int index);

        virtual std::vector<std::string> getLayersName() const = 0;
        virtual void addLayer(const std::string& layerName) = 0;
        virtual void removeLayer(const std::string& layerName) = 0;

        virtual LightProperties* getLight(size_t index) = 0;
        virtual const LightProperties* getLight(size_t index) const = 0;
        virtual void setLight(size_t index, const LightProperties& light) = 0;

        virtual bool isLayerVisible(const std::string& layer_name) const = 0;
        virtual void setLayerVisibility(const std::string& layer_name, bool isVisible) = 0;

        virtual int getLayerCount() const = 0;
        virtual int getLightCount() const = 0;

        virtual std::shared_ptr<Texture2D> getTexture(FrameTextureType type) const = 0;
        virtual void setTexture(FrameTextureType type, std::shared_ptr<Texture2D> texture) = 0;

        int getIndex() const { return index; }
    protected:
        int index{-1};
    };


    class FrameImpl : public Frame
    {
    public:
        FrameImpl();

        // Inherited via Frame

        //these 3 methode has no implemetnation, make these 3
        virtual std::vector<std::string> getLayersName() const override;
        virtual void addLayer(const std::string& layerName) override;
        virtual void removeLayer(const std::string& layerName) override;

        virtual LightProperties* getLight(size_t index) override;
        virtual const LightProperties* getLight(size_t index) const override;
        virtual void setLight(size_t index, const LightProperties& light) override;

        virtual int getLayerCount() const override;
        virtual int getLightCount() const override;

        virtual bool isLayerVisible(const std::string& layer_name) const override;
        virtual void setLayerVisibility(const std::string& layer_name, bool isVisible) override;

        // Updated the type to FrameTextureType
        virtual std::shared_ptr<Texture2D> getTexture(FrameTextureType type) const override;
        virtual void setTexture(FrameTextureType type, std::shared_ptr<Texture2D> texture) override;

    private:
        std::vector<std::string> layers;
        std::unordered_map<std::string, bool> visibleLayers;
        LightProperties lights[max_light];
        int light_count{ 0 };

        std::shared_ptr<Texture2D> textures[1]; // RayTracedResult
    };
}
