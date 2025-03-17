#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <thsan/graphics/texture.h>
#include <thsan/graphics/layer.h>

/*
    TODO:
        - load the .json file content in the folder
        - use the .json file to modify the width and height
        - for each call of "loadLayerFromFolder" remove everything that existed before, this mean becareful with frames as they are decoupled from the layer
*/

namespace RessourceManager
{
    class LayerManager
    {
    public:
        LayerManager() = default;
        static std::shared_ptr<Thsan::Layer> loadLayerFromFolder(const std::string& folderPath);
        static std::shared_ptr<Thsan::Layer> get(const std::string& layerName);
        static std::vector<std::string> getLayerNames();
        static int getMaxIndex();
        static void removeUnused();

        static int width;
        static int height;

    private:
        static int id_gen;
        static std::unordered_map<std::string, std::shared_ptr<Thsan::Layer>> p_layers;
        static void loadTexturesForLayer(const std::string& folderPath, std::shared_ptr<Thsan::Layer> layer);
        static void traverseAndLoadTextures(const std::string& folderPath, std::shared_ptr<Thsan::Layer> layer, Thsan::LayerTextureType type);
    };
}
