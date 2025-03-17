#include "pch.h"
#include "thsan/graphics/frame.h"
#include "thsan/graphics/texture.h"
#include "thsan/log.h"

namespace Thsan
{
    // FrameImpl
    FrameImpl::FrameImpl() : light_count(0) {}


    std::vector<std::string> FrameImpl::getLayersName() const
    {
        return layers;
    }

    void FrameImpl::addLayer(const std::string& layerName) 
    {
        layers.push_back(layerName);
        visibleLayers[layerName] = true;
    }

    void FrameImpl::removeLayer(const std::string& layerName)
    {
        auto it = std::find(layers.begin(), layers.end(), layerName);
        if (it != layers.end())
            layers.erase(it);
        visibleLayers.erase(layerName);
    }

    LightProperties* FrameImpl::getLight(size_t index)
    {
        return index < max_light ? &lights[index] : nullptr;
    }

    const LightProperties* FrameImpl::getLight(size_t index) const
    {
        return index < max_light ? &lights[index] : nullptr;
    }

    void FrameImpl::setLight(size_t index, const LightProperties& light)
    {
        if (index < max_light)
        {
            lights[index] = light;
        }
    }

    int FrameImpl::getLayerCount() const
    {
        return layers.size();
    }

    int FrameImpl::getLightCount() const
    {
        return light_count;
    }

    bool FrameImpl::isLayerVisible(const std::string& layer_name) const
    {
        auto it = std::find(layers.begin(), layers.end(), layer_name);
        if (it != layers.end())
            return visibleLayers.at(layer_name);
        else
            return false;
    }

    void FrameImpl::setLayerVisibility(const std::string& layer_name, bool isVisible)
    {
        auto it = std::find(layers.begin(), layers.end(), layer_name);
        if (it != layers.end())
            visibleLayers[layer_name] = isVisible;
    }

    std::shared_ptr<Texture2D> FrameImpl::getTexture(FrameTextureType type) const
    {
        return textures[static_cast<int>(type)];
    }

    void FrameImpl::setTexture(FrameTextureType type, std::shared_ptr<Texture2D> texture)
    {
        textures[static_cast<int>(type)] = std::move(texture);
    }

    std::shared_ptr<Frame> Frame::create(int index)
    {
        std::shared_ptr<Frame> f = std::make_shared<FrameImpl>();
        f->index = index;
        return std::move(f);
    }
}
