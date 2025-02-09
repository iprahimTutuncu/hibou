#pragma once

#include <core/game_object/component/render_component.h>
#include <core/game_object/data/data.h>
#include <glm/vec3.hpp>

namespace Thsan 
{
    class Texture2D;
    class SpriteAnimation;
}
struct HitBox;

class PlayerSpriteRenderComponent : public RenderComponent
{
    public:
        PlayerSpriteRenderComponent();

        void init(Scene& scene) override;
        void onStart(Scene& scene, Thsan::RendererTerrain& renderer) override;
        void onUpdate(Scene& scene, const float& dt, Thsan::RendererTerrain& renderer) override;

        ~PlayerSpriteRenderComponent() = default;
    private:
        std::shared_ptr<Thsan::SpriteAnimation> playerSprite;
        std::shared_ptr<Thsan::Texture2D> playerTexture;
        //std::shared_ptr<SpriteAnimation> slashSprite;
        //std::shared_ptr<SpriteAnimation> plantSprite;

		bool isLookingLeft;

        HitBox attack_hitbox_start;
        HitBox attack_hitbox_mid;
};
