#pragma once
#include <string>
#include "../system/vector.h"
#include "drawable.h"
#include "tsm/math/color.h"
#include "Transformable.h"

namespace Thsan {

    struct THSAN_API Tileset {
        int tilemapWidth;
        int tilemapHeight;
        int tilewidth;
        int tileheight;
        int margin;
        int spacing;
        tsm::Color backgroundcolor;
    };

    class THSAN_API Tilemap : public Drawable, public Transformable {
    public:
        virtual ~Tilemap() = default;

        virtual bool load(const int* data) = 0;

        virtual bool loadTileset(Tileset tileset, std::shared_ptr<Texture2D> tileset_texture) = 0;

        virtual void setSize(unsigned int width, unsigned int height) = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;

        virtual void setTileSize(unsigned int width, unsigned int height) = 0;

        virtual unsigned int getTileWidth() const = 0;
        virtual unsigned int getTileHeight() const = 0;

        virtual int getTileAt(unsigned int x, unsigned int y) const = 0;
        virtual void setTileAt(unsigned int x, unsigned int y, int tileID) = 0;

        virtual tsm::Color getTileColorAt(unsigned int x, unsigned int y) const = 0;
        virtual void setTileColorAt(unsigned int x, unsigned int y, tsm::Color color) = 0;

        virtual void clear() = 0;

        virtual void resize(unsigned int newWidth, unsigned int newHeight) = 0;

        virtual int* getRawTileData() = 0;


        //inherited from transformable
        virtual void setPosition(const glm::vec3& position) = 0;
        virtual glm::vec3 getPosition() const = 0;

        virtual void setRotation(const glm::vec3& rotate, float angle) = 0;
        virtual glm::mat4 getRotation() const = 0;

        virtual void setScale(const glm::vec3& scale) = 0;
        virtual glm::vec3 getScale() const = 0;

        virtual void setOrigin(const glm::vec3& origin) = 0;
        virtual glm::vec3 getOrigin() const = 0;

        virtual void move(const glm::vec3& offset) = 0;
        virtual void scale(const glm::vec3& scaleFactor) = 0;
        virtual void rotate(const glm::vec3& axis, float angleDegrees) = 0;


    protected:
        // Hérité via Drawable
        virtual void draw(const RenderTarget& target, RenderStates2D& states) const = 0;

    };


    inline THSAN_API std::shared_ptr<Tilemap> create_tilemap();
}
