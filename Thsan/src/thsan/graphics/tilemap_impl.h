#pragma once
#include "tilemap.h"

namespace tsm {
    class Color;
    class Transform;
}

namespace Thsan {

    class TilemapImpl : public Tilemap {
    public:
        // Constructor
        TilemapImpl();

        // Destructor
        virtual ~TilemapImpl() = default;

        // Inherited from Tilemap
        bool load(const int* data) override;
        void setSize(unsigned int width, unsigned int height) override;
        unsigned int getWidth() const override;
        unsigned int getHeight() const override;
        void setTileSize(unsigned int width, unsigned int height) override;
        unsigned int getTileWidth() const override;
        unsigned int getTileHeight() const override;
        int getTileAt(unsigned int x, unsigned int y) const override;
        void setTileAt(unsigned int x, unsigned int y, int tileID) override;
        tsm::Color getTileColorAt(unsigned int x, unsigned int y) const override;
        void setTileColorAt(unsigned int x, unsigned int y, tsm::Color color) override;
        void clear() override;
        void resize(unsigned int newWidth, unsigned int newHeight) override;
        int* getRawTileData() override;
        bool loadTileset(Tileset tileset, std::shared_ptr<Texture2D> tileset_texture) override;

        void draw(const RenderTarget& target, RenderStates2D& states) const override;

        //inherited from transformable
        virtual void setPosition(const glm::vec3& position) override;
        virtual glm::vec3 getPosition() const override;

        virtual void setRotation(const glm::vec3& rotate, float angle) override;
        virtual glm::mat4 getRotation() const override;

        virtual void setScale(const glm::vec3& scale) override;
        virtual glm::vec3 getScale() const override;

        virtual void setOrigin(const glm::vec3& origin) override;
        virtual glm::vec3 getOrigin() const override;

        virtual void move(const glm::vec3& offset) override;
        virtual void scale(const glm::vec3& scaleFactor) override;
        virtual void rotate(const glm::vec3& axis, float angleDegrees) override;


        virtual glm::mat4 getTranform() override;

    private:
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Texture2D> tileset_texture;

        // Tilemap data
        std::vector<int> tileData;
        std::unordered_map<int, vec2f> tileset_coordinates;
        uint32_t tilemapWidth{0};
        uint32_t tilemapHeight{0};
        uint32_t tileWidth{0};
        uint32_t tileHeight{0};
        int margin{};
        int spacing{};
        tsm::Color keyColor;

        std::shared_ptr<tsm::Transform> transform;
    };
}