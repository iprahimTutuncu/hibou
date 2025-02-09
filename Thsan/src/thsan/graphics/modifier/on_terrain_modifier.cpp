#include "pch.h"

#include <thsan/graphics/mesh.h>
#include <thsan/graphics/modifier/on_terrain_modifier.h>
#include <thsan/graphics/view.h>
#include <thsan/graphics/drawable/sprite_animation.h>
#include <thsan/ressource_manager/texture2D_manager.h>
#include <thsan/log.h>
#include <thsan/options.h>

Thsan::OnTerrainModifierImpl::OnTerrainModifierImpl(int terrain_size) :
    OnTerrainModifier(terrain_size)
{
}

void Thsan::OnTerrainModifierImpl::setTerrainOptions(TerrainOptions desc, WindowOption windowOptions)
{   
    for (int i = 0; i < 3; ++i)
        this->terrainDesc.position[i] = desc.position[i];

    this->terrainDesc.normalBlurr[0] = desc.normalBlurr[0];
    this->terrainDesc.normalBlurr[1] = desc.normalBlurr[1];
    this->terrainDesc.terrainSize = desc.terrainSize;
    this->terrainDesc.normalBlurIteration = desc.normalBlurIteration;
    this->terrainDesc.LOD = desc.LOD;
    this->terrainDesc.LODStartAt = desc.LODStartAt;
    this->terrainDesc.angle = desc.angle;
    this->terrainDesc.horizon = desc.horizon;
    this->terrainDesc.scaleHeight = desc.scaleHeight;
    this->terrainDesc.spriteRenderDistance = desc.spriteRenderDistance;
    this->terrainDesc.terrainRenderDistance = desc.terrainRenderDistance;
    this->terrainDesc.shininess = desc.shininess;
    this->terrainDesc.blurType = desc.blurType;

    strcpy_s(this->terrainDesc.albedoTexturePath, desc.albedoTexturePath);
    strcpy_s(this->terrainDesc.heightTexturePath, desc.heightTexturePath);
    strcpy_s(this->terrainDesc.collisionHeightTexturePath, desc.collisionHeightTexturePath);
    strcpy_s(this->terrainDesc.collisionMaskTexturePath, desc.collisionMaskTexturePath);

    screenWidth = static_cast<float>(windowOptions.screenWidth);

}

void Thsan::OnTerrainModifierImpl::setView(std::shared_ptr<View> view)
{
    pView = view;
}

//add rajoute à une liste
//cette list on push et on update a chaque fois

//ou encore

//cette list est utiliser on modifie le content
//...
//yo on retire les elements qui sont que du copy/paste
//tho non
//tho oui, on va toujours re-calc tout anyway lol

void Thsan::OnTerrainModifierImpl::add(std::shared_ptr<SpriteAnimation> spriteAnimation)
{
    sprites.push_back(spriteAnimation);
}

void Thsan::OnTerrainModifierImpl::update()
{
    float pi = glm::pi<float>();
    glm::vec2 n = glm::vec2(cos(terrainDesc.angle + pi / 2), sin(terrainDesc.angle + pi / 2));

    for (auto& spriteAnimation : sprites)
    {
        spriteAnimation->hide();

        glm::vec3 position = spriteAnimation->getExternalPosition();
        glm::vec2 scale = spriteAnimation->getExternalScale();
        float rotation = spriteAnimation->getExternalRotation();

        //l'engin peut seulement scale correctement quand les deux composantes sont égaux


        float size = static_cast<float>(terrain_size);
        position.x += size / 2;
        position.z += size / 2;

        float width = spriteAnimation->getFrameWidth() * glm::abs(scale.x);

        SpriteData* sp = new SpriteData{ spriteAnimation, scale, position, rotation, 0, position.x - width / 2, position.x + width / 2, 0, 0 };
        spritesData.push_back(sp);
    }

    for (SpriteData* spriteData : spritesData)
        if (spriteData->spriteAnimation->isHidden()) {

            float rectHeight = spriteData->spriteAnimation->getFrameHeight();
            spriteData->curr_height = spriteData->position.y + (rectHeight/2 * spriteData->scale.y);
            //X axis of device coordinate
            glm::vec2 relative_to_camera = glm::vec2(
                (spriteData->position.x - terrain_size/2.f) - terrainDesc.position[0],
                 terrainDesc.position[2] - (spriteData->position.z - terrain_size / 2.f)
            );

            float position_dot_prod_with_direction = n.x * relative_to_camera.x + n.y * relative_to_camera.y;

            glm::vec2 relative_to_direction = glm::vec2(position_dot_prod_with_direction * n.x, position_dot_prod_with_direction * n.y);

            glm::vec2 x_axis_dist = relative_to_camera - relative_to_direction;
            glm::vec2 distance = glm::vec2(
                sqrt(x_axis_dist.x * x_axis_dist.x + x_axis_dist.y * x_axis_dist.y),
                sqrt(relative_to_direction.x * relative_to_direction.x + relative_to_direction.y * relative_to_direction.y)
            );

            //vérification si la distance est à la droite du centre de l'ecran ou à la gauche.
            float dot = relative_to_camera.x * -n.y + relative_to_camera.y * n.x;
            if (dot > 0)
                distance.x *= -1;

            //vérification si la distance dans la profondeur est positif ou négatif, soit derrière la camera ou non
            dot = relative_to_camera.x * n.x + relative_to_camera.y * n.y;
            if (dot < 0)
                distance.y *= -1;

            float normalized_x_on_screen = (distance.x + distance.y) / (2 * distance.y);

            //Y axis of device coordinate and scale
            spriteData->sprite_z = distance.y;
            spriteData->dx = distance.y * 2 / screenWidth;
            float scale_final = glm::abs(spriteData->scale.x) / spriteData->dx;
            float sprite_height_on_screen = (terrainDesc.position[1] - spriteData->curr_height) / spriteData->sprite_z * terrainDesc.scaleHeight + terrainDesc.horizon;
            
            //sprite transform
            std::shared_ptr<SpriteAnimation> sa = spriteData->spriteAnimation;

            if (distance.y > 0)
                sa->show();

            sa->setRotation(glm::vec3(0.f, 0.f, 1.f), spriteData->rotation);
            sa->setScale(glm::vec3(scale_final * spriteData->scale.x, scale_final * spriteData->scale.y, 1.f));
            sa->setPosition(glm::vec3(normalized_x_on_screen * screenWidth, sprite_height_on_screen, spriteData->sprite_z));
        }

    spritesData.clear();

    if (pView)
        pView->setMaximumDepth(terrainDesc.terrainRenderDistance);

    else
        TS_CORE_WARN("On Thsan::OnTerrainModifierImpl::update(), pView is nullptr, cannot set maximumDepth for the view.");
}

std::shared_ptr<Thsan::OnTerrainModifier> Thsan::OnTerrainModifier::create(int terrain_size)
{
    return std::make_shared<OnTerrainModifierImpl>(terrain_size);
}
