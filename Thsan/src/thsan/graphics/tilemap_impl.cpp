#include "pch.h"
#include "tilemap_impl.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "../log.h"
#include "tsm/math/color.h"
#include "tsm/math/transform.h"

namespace Thsan {

    TilemapImpl::TilemapImpl() {
        mesh = create_mesh(0);
        transform = tsm::Transform::create();
    }

    bool TilemapImpl::load(const int* data) {
        if (!data)
        {
            // Handle invalid input data, e.g., by returning false or throwing an exception
            TS_CORE_ERROR("in TilemapImpl::load(const int* data), data is invalid");
            return false;
        }

        const int totalTiles = tilemapWidth * tilemapHeight;

        tileData.resize(totalTiles);

        mesh = create_mesh(totalTiles * 4);

        std::vector<Vertex> vertices;
        vertices.reserve(totalTiles * 4);
        Thsan::vec4f color = tsm::Color(255, 255, 255, 255).toVec4f();

        // Define an index buffer to create triangles from vertices
        std::vector<uint32_t> indices;
        indices.reserve(totalTiles * 6);

        float tw = static_cast<float>(tileWidth);
        float th = static_cast<float>(tileHeight);

        float image_w = static_cast<float>(tileset_texture->getWidth());
        float image_h = static_cast<float>(tileset_texture->getHeight());


        for (unsigned int i = 0; i < tilemapWidth; ++i)
            for (unsigned int j = 0; j < tilemapHeight; ++j)
            {
                // get the current tile number
                int tileIndex = i + j * tilemapWidth;
                tileData[i] = data[tileIndex];

                vec2f texCoords = tileset_coordinates[tileData[i]];
                Thsan::vec2f texCoordsSize(tileWidth / image_w, tileHeight / image_h);

                if (tileData[i] == 0) {
                    texCoords = Thsan::vec2f(0.f, 0.f);
                    texCoordsSize = Thsan::vec2f(0.f, 0.f);
                }

                // Calculate the base vertex index for the current tile
                int baseVertexIndex = tileIndex * 4;

                // Define the vertices for the current tile with horizontal and vertical translations
                vertices.push_back(Vertex(vec3f(i * tw, j * th, 0.f), vec3f(0.f, 0.f, -1.f), texCoords));
                vertices.back().color = color;

                vertices.push_back(Vertex(vec3f((i + 1) * tw, j * th, 0.f), vec3f(0.f, 0.f, -1.f), vec2f(texCoords.x + texCoordsSize.x, texCoords.y)));
                vertices.back().color = color;

                vertices.push_back(Vertex(vec3f((i + 1) * tw, (j + 1) * th, 0.0f), vec3f(0.f, 0.f, -1.f), vec2f(texCoords.x + texCoordsSize.x, texCoords.y + texCoordsSize.y)));
                vertices.back().color = color;

                vertices.push_back(Vertex(vec3f(i * tw, (j + 1) * th, 0.0f), vec3f(0.f, 0.f, -1.f), vec2f(texCoords.x, texCoords.y + texCoordsSize.y)));
                vertices.back().color = color;

                // Define the indices to create triangles for the current tile
                indices.push_back(baseVertexIndex + 0);
                indices.push_back(baseVertexIndex + 1);
                indices.push_back(baseVertexIndex + 2);
                indices.push_back(baseVertexIndex + 2);
                indices.push_back(baseVertexIndex + 3);
                indices.push_back(baseVertexIndex + 0);
        }

        mesh->setVertices(vertices);
        mesh->setIndices(indices); // Set the indices for rendering

        mesh->generate();

        return true;
    }



    void TilemapImpl::setSize(unsigned int width, unsigned int height) {

        tilemapWidth = width;
        tilemapHeight = height;

        tileData.resize(tilemapWidth * tilemapHeight, -1);
        mesh->resize(tilemapWidth * tilemapHeight * 4);
    }

    unsigned int TilemapImpl::getWidth() const {
        return tilemapWidth;
    }

    unsigned int TilemapImpl::getHeight() const {
        return tilemapHeight;
    }

    void TilemapImpl::setTileSize(unsigned int width, unsigned int height) {
        // Update the tile size if needed
        tileWidth = width;
        tileHeight = height;

        // You may need to update other related data or buffers
    }

    unsigned int TilemapImpl::getTileWidth() const {
        return tileWidth;
    }

    unsigned int TilemapImpl::getTileHeight() const {
        return tileHeight;
    }

    int TilemapImpl::getTileAt(unsigned int x, unsigned int y) const {

        if (x < tilemapWidth && y < tilemapHeight) {
            int index = y * tilemapWidth + x;
            return tileData[index];
        }
        else {
            TS_CORE_WARN("In TilemapImpl::getTileAt(unsigned int x, unsigned int y), coordinates are out of bounds.");
            return -1; // Return a default value or handle out-of-bounds as needed
        }
    }

    void TilemapImpl::setTileAt(unsigned int x, unsigned int y, int tileID) {
      
        if (x < tilemapWidth && y < tilemapHeight) {
            int index = x + y * tilemapWidth;
            tileData[index] = tileID;

            float tw = static_cast<float>(tileWidth);
            float th = static_cast<float>(tileHeight);

            float image_w = static_cast<float>(tileset_texture->getWidth());
            float image_h = static_cast<float>(tileset_texture->getHeight());

            vec2f texCoords = tileset_coordinates[tileID];
            Thsan::vec2f texCoordsSize(tw / image_w, th / image_h);

            if (tileID == 0) {
                texCoords = Thsan::vec2f(0.f, 0.f);
                texCoordsSize = Thsan::vec2f(0.f, 0.f);
            }

            // Calculate the base vertex index for the current tile
            int baseVertexIndex = index * 4;

            // Define the vertices for the current tile with horizontal and vertical translations
            (*mesh)[index].texCoord = texCoords;
            (*mesh)[index].texCoord = vec2f{ texCoords.x + texCoordsSize.x, texCoords.y };
            (*mesh)[index].texCoord = vec2f{ texCoords.x + texCoordsSize.x, texCoords.y + texCoordsSize.y };
            (*mesh)[index].texCoord = vec2f{ texCoords.x, texCoords.y + texCoordsSize.y };
            
            mesh->generate();
        }
        else {
            TS_CORE_WARN("In TilemapImpl::setTileAt(unsigned int x, unsigned int y, int tileID), coordinates are out of bounds.");
        }
    }

    tsm::Color TilemapImpl::getTileColorAt(unsigned int x, unsigned int y) const {

        if (x < tilemapWidth && y < tilemapHeight) {
            int index = y * tilemapWidth + x;
            int meshIndex = index * 4;

            if (meshIndex >= 0 && meshIndex < mesh->getVertexCount())
                return tsm::Color::fromVec4f((*mesh)[meshIndex].color);
            else {
                TS_CORE_WARN("In TilemapImpl::getTileColorAt(unsigned int x, unsigned int y), mesh index is out of bounds.");
            }
        }
        else {
            TS_CORE_WARN("In TilemapImpl::getTileColorAt(unsigned int x, unsigned int y), coordinates are out of bounds.");
        }

        return tsm::Color(255, 255, 255, 255); // Default white color
    }

    void TilemapImpl::setTileColorAt(unsigned int x, unsigned int y, tsm::Color color) {

        if (x < tilemapWidth && y < tilemapHeight) {
            int index = y * tilemapWidth + x;
            int meshIndex = index * 4;

            if (meshIndex >= 0 && meshIndex < mesh->getVertexCount()) {
                for (int i = 0; i < 4; i++)
                    (*mesh)[meshIndex + i].color = color.toVec4f();
               
                mesh->generate();
            }
            else {
                TS_CORE_WARN("In TilemapImpl::setTileColorAt(unsigned int x, unsigned int y, vec4f color), mesh index is out of bounds.");
            }
        }
        else {
            TS_CORE_WARN("In TilemapImpl::setTileColorAt(unsigned int x, unsigned int y, vec4f color), coordinates are out of bounds.");
        }
    }


    void TilemapImpl::clear()
    {
    }

    void TilemapImpl::resize(unsigned int newWidth, unsigned int newHeight)
    {
        std::vector<int> newTilemap(newWidth * newHeight);

        for (unsigned int y = 0; y < std::min(tilemapHeight, newHeight); ++y)
            for (unsigned int x = 0; x < std::min(tilemapWidth, newWidth); ++x)
                newTilemap[y * tilemapWidth + x] = tileData[y * tilemapWidth + x];

        // Update dimensions and replace the old tilemap
        tilemapWidth = newWidth;
        tilemapHeight = newHeight;
        tileData = std::move(newTilemap);
        mesh->resize(newWidth * newHeight * 4);
    }


    int* TilemapImpl::getRawTileData()
    {
        return tileData.data();
    }

    // Implement the rest of the virtual functions

    bool TilemapImpl::loadTileset(Tileset tileset, std::shared_ptr<Texture2D> tileset_texture)
    {
        if (!tileset_texture)
        {
            TS_CORE_WARN("In TilemapImpl::loadTileset(std::shared_ptr<Texture2D> tileset_texture), trying to pass a nullptr tileset_texture.");
            return false;
        }

        this->tileset_texture = tileset_texture;

        int image_width = static_cast<int>(tileset_texture->getWidth());
        int image_height = static_cast<int>(tileset_texture->getHeight());

        margin = tileset.margin;
        spacing = tileset.spacing;

        // Calculate the number of rows and columns in the tileset
        int columns = (tileset_texture->getSize().x - margin) / tileset.tilewidth;
        int rows = (tileset_texture->getSize().y - margin) / tileset.tileheight;

        tilemapWidth = tileset.tilemapWidth;
        tilemapHeight = tileset.tilemapHeight;

        // Calculate tile width and height
        tileWidth = tileset.tilewidth;
        tileHeight = tileset.tileheight;

        // Set the key color based on background color
        keyColor = tileset.backgroundcolor;

        // Fill in the tileset_coordinates map
        tileset_coordinates.clear();
        int tileIndex = 0;

        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                // Calculate the position of the tile in the tileset texture
                int startX = tileset.margin + x * (tileset.tilewidth + tileset.spacing);
                int startY = tileset.margin + y * (tileset.tileheight + tileset.spacing);

                float norm_start_x = static_cast<float>(startX) / static_cast<float>(image_width);
                float norm_start_y = static_cast<float>(startY) / static_cast<float>(image_height);

                // Store the tile's position in the map
                tileset_coordinates[++tileIndex] = vec2f(norm_start_x, norm_start_y);
            }
        }

        return true;
    }

    void TilemapImpl::draw(const RenderTarget& target, RenderStates2D& states) const
    {
        auto s = states.getShader().lock();
        s->setVec4("key_color", keyColor.toGlm());
        s->setMat4("transform", transform->getTransform());
        s->setTexture2D("default_texture", tileset_texture);
        mesh->bind();
        target.draw(*mesh, states);
        mesh->unbind();
    }



    void TilemapImpl::setPosition(const glm::vec3& position)
    {
        transform->setTranslation(position);
    }

    glm::vec3 TilemapImpl::getPosition() const
    {
        return transform->getTranslation();
    }

    void TilemapImpl::setRotation(const glm::vec3& rotate, float angle)
    {
        transform->setRotation(rotate, angle);
    }

    glm::mat4 TilemapImpl::getRotation() const
    {
        return transform->getRotation();
    }

    void TilemapImpl::setScale(const glm::vec3& scale)
    {
        transform->scale(scale);
    }

    glm::vec3 TilemapImpl::getScale() const
    {
        return transform->getScale();
    }

    void TilemapImpl::setOrigin(const glm::vec3& origin)
    {
        transform->setOrigin(origin);
    }

    glm::vec3 TilemapImpl::getOrigin() const
    {
        return transform->getOrigin();
    }

    void TilemapImpl::move(const glm::vec3& offset)
    {
        transform->translate(offset);
    }

    void TilemapImpl::scale(const glm::vec3& scaleFactor)
    {
        transform->scale(scaleFactor);
    }

    void TilemapImpl::rotate(const glm::vec3& axis, float angleDegrees)
    {
        transform->rotate(axis, angleDegrees);
    }
    glm::mat4 TilemapImpl::getTranform()
    {
        return transform->getTransform();
    }
}
