#include "pch.h"
#include "thsan/graphics/layer.h"
#include "thsan/log.h"

namespace Thsan
{
    std::vector<std::string> LayerImpl::existing_name;

    // LayerImpl
    LayerImpl::LayerImpl() : depth_value(0.0f) {}

    std::shared_ptr<Texture2D> LayerImpl::getTexture(LayerTextureType type, size_t index) const
    {
        return textures[static_cast<int>(type)][index];
    }

    void LayerImpl::setTexture(LayerTextureType type, std::shared_ptr<Texture2D> texture, size_t index)
    {
        textures[static_cast<int>(type)][index-1] = texture;
    }

    float LayerImpl::getDepthValue() const
    {
        return depth_value;
    }

    void LayerImpl::setDepthValue(float value)
    {
        depth_value = value;
    }

    std::string LayerImpl::getName() const
    {
        return name;
    }

    void LayerImpl::setName(std::string new_name)
    {
        if (std::find(existing_name.begin(), existing_name.end(), new_name) != existing_name.end())
        {
            TS_CORE_ERROR("Error: Layer name {} already exists!", new_name);
            return;
        }

        if (!name.empty())
        {
            existing_name.erase(std::remove(existing_name.begin(), existing_name.end(), name), existing_name.end());
        }

        name = new_name;
        existing_name.push_back(name);
    }

    size_t LayerImpl::getMaximumIndex()
    {
        return textures[0].size();
    }

    void LayerImpl::setMaximumIndex(size_t index)
    {
        for (int i = 0; i < max_texture_type_count; i++)
        {
            textures[i].resize(index);
        }
    }

    std::shared_ptr<Layer> Layer::create()
    {
        return std::make_shared<LayerImpl>();
    }
}