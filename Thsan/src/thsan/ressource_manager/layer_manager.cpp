#include "pch.h"
#include "thsan/ressource_manager/layer_manager.h"
#include "thsan/log.h"
#include "thsan/ressource_manager/texture2D_manager.h"
#include "thsan/graphics/texture.h"

namespace RessourceManager
{
    //TODO, have this info in the .json and load it
    int RessourceManager::LayerManager::width = 2880;
    int RessourceManager::LayerManager::height = 2160;

    int RessourceManager::LayerManager::id_gen = 0;

    std::unordered_map<std::string, std::shared_ptr<Thsan::Layer>> LayerManager::p_layers;

    std::shared_ptr<Thsan::Layer> LayerManager::loadLayerFromFolder(const std::string& folderPath)
    {
        // Check if layer already loaded
        std::filesystem::path path(folderPath);
        std::string folderName = path.filename().string();

        auto it = p_layers.find(folderName);
        if (it != p_layers.end())
            return it->second;

        // Create new layer
        std::shared_ptr<Thsan::Layer> layer = Thsan::Layer::create();
        loadTexturesForLayer(folderPath, layer);

        p_layers.insert({ folderName, layer });
        return layer;
    }

    std::shared_ptr<Thsan::Layer> LayerManager::get(const std::string& layerName)
    {
        auto it = p_layers.find(layerName);
        if (it != p_layers.end())
            return it->second;

        return nullptr;
    }

    std::vector<std::string> LayerManager::getLayerNames()
    {
        std::vector<std::string> layerNames;

        for (const auto& layer : p_layers)
        {
            layerNames.push_back(layer.first);
        }

        return layerNames;
    }

    int LayerManager::getMaxIndex()
    {
        int i = 0;
        std::vector<std::string> names = getLayerNames();
        for (std::string name : names)
        {
            int t = get(name)->getMaximumIndex();
            if (i < t)
                i = t;
        }

        return i;
    }


    void LayerManager::removeUnused()
    {
        for (auto i = p_layers.begin(); i != p_layers.end();)
        {
            if (i->second.use_count() == 1)
                i = p_layers.erase(i);
            else
                ++i;
        }
    }

    void LayerManager::loadTexturesForLayer(const std::string& folderPath, std::shared_ptr<Thsan::Layer> layer)
    {
        for (int i = 0; i < 3; ++i)
        {
            std::string textureFolder;
            switch (static_cast<Thsan::LayerTextureType>(i))
            {
            case Thsan::LayerTextureType::Mask:
                textureFolder = folderPath + "/mask";
                break;
            case Thsan::LayerTextureType::Diffuse:
                textureFolder = folderPath + "/diffuse";
                break;
            case Thsan::LayerTextureType::Depth:
                textureFolder = folderPath + "/depth";
                break;
            }

            traverseAndLoadTextures(textureFolder, layer, static_cast<Thsan::LayerTextureType>(i));
        }

        RessourceManager::TextureDesc desc;
        desc.data = nullptr;
        desc.format = Thsan::TextureFormat::R16;
        desc.size = glm::uvec2(RessourceManager::LayerManager::width, RessourceManager::LayerManager::height);

        auto modifiedDepth = RessourceManager::Texture2DManager::create("layer" + std::to_string(id_gen++) + "_modifiedDepth", desc);
        layer->setTexture(Thsan::LayerTextureType::ModifiedDepth, modifiedDepth, layer->getMaximumIndex());

    }

    void LayerManager::traverseAndLoadTextures(const std::string& folderPath, std::shared_ptr<Thsan::Layer> layer, Thsan::LayerTextureType type)
    {
        std::filesystem::path path(folderPath);
        size_t index = 1;

        std::vector<size_t> indices;

        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();
                size_t number_start = filename.find_first_of('-') + 1;
                size_t number_end = filename.find_last_of('-');

                if (number_start != std::string::npos && number_end != std::string::npos)
                {
                    size_t indexFromFilename = std::stoi(filename.substr(number_start, number_end - number_start));
                    indices.push_back(indexFromFilename);
                }
            }
        }

        if (!indices.empty())
        {
            index = *std::max_element(indices.begin(), indices.end());
        }

        layer->setMaximumIndex(index);

        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();
                size_t number_start = filename.find_first_of('-') + 1;
                size_t number_end = filename.find_last_of('-');

                if (number_start != std::string::npos && number_end != std::string::npos)
                {
                    size_t indexFromFilename = std::stoi(filename.substr(number_start, number_end - number_start));

                    if (indexFromFilename == index)
                    {
                        std::shared_ptr<Thsan::Texture2D> texture;

                        if (type != Thsan::LayerTextureType::Depth && type != Thsan::LayerTextureType::ModifiedDepth)
                        {
                           texture = RessourceManager::Texture2DManager::get(entry.path().string());
                        }
                        else
                        {
                           texture = RessourceManager::Texture2DManager::get(entry.path().string(), false, Thsan::TextureBitDepth::BitDepth16);
                        }

                        layer->setTexture(type, texture, index);

                        index++;
                    }
                    else if (indexFromFilename > index)
                    {
                        layer->setTexture(type, nullptr, index);
                        index = indexFromFilename;
                    }
                }
            }
        }
    }

}
