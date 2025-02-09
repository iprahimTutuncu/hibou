#include "player_sprite_render_component.h"
#include "core/scene.h"
#include "core/game_object/game_object.h"
#include "core/game_object/data/data.h"

#include <glm/glm.hpp>
#include <thsan/graphics/drawable/sprite_animation.h>
#include <thsan/graphics/texture.h>
#include <thsan/state/state.h>
#include <thsan/game.h>

PlayerSpriteRenderComponent::PlayerSpriteRenderComponent():
	isLookingLeft(true)
{

}

void PlayerSpriteRenderComponent::init(Scene& scene)
{
    //todo in 2000 years,
    //refactor this, texture manager should be some fake singleton that reference the textureManager in game.h
    //includeing game.h directly in this level, is just weird
    playerTexture = scene.getParent()->getParent()->getTexture("media/image/sonic.png");
    playerSprite = Thsan::SpriteAnimation::create(playerTexture);

    if (!parent->hasComponent(DataType::PHYSIC_BODY)) {
        parent->setComponent(DataType::PHYSIC_BODY, new PhysicBodyData());
    }

    if (!parent->hasComponent(DataType::PLAYER)) {
        parent->setComponent(DataType::PLAYER, new PlayerData());
    }

    if (!parent->hasComponent(DataType::BAG)) {
        parent->setComponent(DataType::BAG, new BagData());
    }

    Transform* temp = parent->getComponent<Transform>(DataType::TRANSFORM);
    temp->scale.x = 0.45;
    temp->scale.y = 0.45;

    //todo in 2000 years, have shininess on SpriteAnimaiton
   // playerSprite->setShininess(32.f);

    playerSprite->setKeyColor(tsm::Color(0, 112, 112, 255));
    playerSprite->setCurrAnimation("idle");
    playerSprite->add(glm::vec4(11, 14, 22, 32), 0.1f);
    playerSprite->add(glm::vec4(38, 12, 22, 34), 0.1f);
    playerSprite->add(glm::vec4(65, 11, 24, 35), 0.1f);
    playerSprite->add(glm::vec4(123, 14, 26, 32), 0.1f);
    playerSprite->add(glm::vec4(154, 15, 24, 31), 0.1f);

    playerSprite->setCurrAnimation("jumping");
    playerSprite->add(glm::vec4(9, 272, 22, 40),   0.1f);
    playerSprite->add(glm::vec4(36, 272, 22, 40),  0.1f);
    playerSprite->add(glm::vec4(63, 274, 24, 36),  0.1f);
    playerSprite->add(glm::vec4(92, 277, 23, 34),  0.1f);
    playerSprite->add(glm::vec4(120, 280, 22, 31), 0.1f);

    playerSprite->setCurrAnimation("double-jumping");
    playerSprite->add(glm::vec4(14, 413, 33, 33), 0.1f);
    playerSprite->add(glm::vec4(52, 413, 33, 33), 0.1f);
    playerSprite->add(glm::vec4(90, 414, 31, 31), 0.1f);
    playerSprite->add(glm::vec4(126, 416, 29, 27),0.1f);
    playerSprite->add(glm::vec4(162, 414, 26, 31),0.1f);
    playerSprite->add(glm::vec4(194, 411, 26, 36),0.1f);
    playerSprite->add(glm::vec4(226, 410, 26, 36),0.1f);

    playerSprite->setCurrAnimation("idle-to-run");
    playerSprite->add(glm::vec4(10, 101, 24, 33), 0.1f);

    playerSprite->setCurrAnimation("run");
    playerSprite->add(glm::vec4(45, 100, 31, 32),  0.1f);
    playerSprite->add(glm::vec4(81, 101, 30, 32),  0.1f);
    playerSprite->add(glm::vec4(116, 100, 23, 34), 0.1f);
    playerSprite->add(glm::vec4(144, 99, 28, 35),  0.1f);
    playerSprite->add(glm::vec4(177, 101, 30, 33), 0.1f);
    playerSprite->add(glm::vec4(212, 101, 28, 33), 0.1f);
    playerSprite->add(glm::vec4(245, 100, 23, 34), 0.1f);
    playerSprite->add(glm::vec4(273, 99, 28, 35),  0.1f);

    playerSprite->setCurrAnimation("landing");
    playerSprite->add(glm::vec4(11, 370, 27, 30), 0.1f);
    playerSprite->add(glm::vec4(43, 370, 26, 30), 0.1f);
    playerSprite->add(glm::vec4(74, 370, 22, 30), 0.1f);

    playerSprite->setCurrAnimation("falling");
    playerSprite->add(glm::vec4(12, 325, 24, 34),  0.1f);
    playerSprite->add(glm::vec4(42, 323, 30, 36),  0.1f);
    playerSprite->add(glm::vec4(77, 321, 35, 39),  0.1f);
    playerSprite->add(glm::vec4(117, 320, 35, 40), 0.1f);

    playerSprite->setCurrAnimation("charging");
    playerSprite->add(glm::vec4(165, 1066, 20, 24), 0.05f);
    playerSprite->add(glm::vec4(191, 1065, 20, 25), 0.05f);
    playerSprite->add(glm::vec4(218, 1067, 20, 22), 0.05f);
    playerSprite->add(glm::vec4(246, 1068, 20, 20), 0.05f);

    //todo, quand le sprite sera terminé, avoir un sprite pour
	//playerSprite->setDepthTexture("media/image/base0.png");

    /*
    *  animation for planting, might not actually be needed as in ideal world it's the job of
    *  the playerSprite, that said, some sparkles might be nice
    * 
    plantSprite = createSpriteAnimation("media/image/effect/slash_fire.png", "plantSprite");

    plantSprite->enableLoop(false);
    plantSprite->setKeyColor(sf::Color(0, 0, 0, 0));
    plantSprite->setCurrAnimation("plant");
    plantSprite->addFrame("plant", sf::IntRect(0, 0, 1, 1), sf::seconds(0.1f));
    */
    ////////////////////////////////////////////
    // set up hitbox and animation of attacks //
    //////////////////////////////////////////// 
    /*
    * 
    * Todo in 2000 years,
    * port tous les trucs ayant un lien avec les avant le call d'event des frame de sprite
    * penser commetn intérgrer ca
    * 
    slashSprite = createSpriteAnimation("media/image/effect/slash_fire.png", "slashSprite");
    attack_hitbox_start.spheres.push_back(Sphere{ 50.f, glm::vec3(0.f) });
    attack_hitbox_mid.spheres.push_back(Sphere{ 200.f, glm::vec3(0.f) });

    slashSprite->enableLoop(false);
    slashSprite->setKeyColor(tsm::Color(0, 0, 0, 0));
    slashSprite->setCurrAnimation("slash");
    slashSprite->add(glm::vec4(215, 201, 127, 119), 0.1f);
    slashSprite->add(glm::vec4(170 * 1, 172 * 1, 170 * 1, 172 * 1), 0.05f);
    slashSprite->add(glm::vec4(170 * 2, 172 * 1, 170 * 1, 172 * 1), 0.05f);
    slashSprite->add(glm::vec4(170 * 3, 172 * 1, 170 * 1, 172 * 1), 0.05f);
    slashSprite->add(glm::vec4(170 * 4, 172 * 1, 170 * 1, 172 * 1), 0.05f);
    slashSprite->add(glm::vec4(170 * 0, 172 * 2, 170 * 1, 172 * 1), 0.05f);
    slashSprite->add(glm::vec4(170 * 1, 172 * 2, 170 * 1, 172 * 1), 0.05f);
    slashSprite->add(glm::vec4(170 * 2, 172 * 2, 170 * 1, 172 * 1), 0.05f);
    slashSprite->add(glm::vec4(170 * 3, 172 * 2, 170 * 1, 172 * 1), 0.05f);


    //make these line of code exist after the animation
    parent->addOnSpriteAnimationRenderEvent("slash", 0.2f, false, [&](GameObject& go, Scene& scene, const float& at) {

        Transform* tr_player = go.getComponent<Transform>(DataType::TRANSFORM);

        GameObject* cam_go = scene.get_child("camera");
        float camera_angle{ 0.f };
        if (cam_go) {
            camera_angle = cam_go->getComponent<CameraData>(DataType::CAMERA)->angle;
        }
        const double pi = 3.14159265358979323846;

        glm::vec3 n = glm::normalize(glm::vec3(sin(camera_angle + pi / 2), 0.f, cos(camera_angle + pi / 2)));
        if (playerSprite->isFlipX())
            n = -n;
        n *= 10.0f;

        attack_hitbox_start.position = n;
        
        CollisionEngine::add("player_attack_hitbox", tr_player, &attack_hitbox_start);
    });

    parent->addOnSpriteAnimationRenderEvent("slash", 0.7f, false, [&](GameObject& go, Scene& scene, const float& at) {

        Transform* tr_player = go.getComponent<Transform>(DataType::TRANSFORM);

        GameObject* cam_go = scene.get_child("camera");
        float camera_angle{ 0.f };
        if (cam_go) {
            camera_angle = cam_go->getComponent<CameraData>(DataType::CAMERA)->angle;
        }
        const double pi = 3.14159265358979323846;

        glm::vec3 n = glm::normalize(glm::vec3(sin(camera_angle + pi / 2), 0.f, cos(camera_angle + pi / 2)));
        if (playerSprite->isFlipX())
            n = -n;
        n *= 10.0f;

        attack_hitbox_mid.position = n;
        CollisionEngine::remove("player_attack_hitbox");
        CollisionEngine::add("player_attack_hitbox", tr_player, &attack_hitbox_mid);
    });

    */

}

void PlayerSpriteRenderComponent::onStart(Scene& scene, Thsan::RendererTerrain& renderer)
{
    playerSprite->start();
    renderer.add(playerSprite, parent->getName());

    //yo low-key forgot why
    playerSprite->update(0.1f);
}

void PlayerSpriteRenderComponent::onUpdate(Scene& scene, const float& dt, Thsan::RendererTerrain& renderer)
{
    //renderer.enableDebugRendering(Debug_rendering::debug_normal);
    Transform* temp = parent->getComponent<Transform>(DataType::TRANSFORM);
    PlayerData* player_data = parent->getComponent<PlayerData>(DataType::PLAYER);

	glm::vec3 direction = parent->getComponent<PhysicBodyData>(DataType::PHYSIC_BODY)->direction;

    glm::vec3 position = temp->position;
    

	static float t = 0;
	t += dt;

    playerSprite->update(dt);


    //set player spirte animation
	glm::vec3 dir = glm::vec3(direction.x, direction.y, direction.z);

    if (player_data->curr_state == PlayerData::State::charge)
    {
        playerSprite->setCurrAnimation("charging");
        playerSprite->enableLoop();
    }
    else if (player_data->curr_state == PlayerData::State::jump) {
        if (playerSprite->getCurrentAnimation() != "jumping") {
            playerSprite->setCurrAnimation("jumping");
            playerSprite->resetCurrAnimation();
        }
        playerSprite->enableLoop();
    }
    else if (player_data->curr_state == PlayerData::State::double_jump) {
        if (playerSprite->getCurrentAnimation() != "double-jumping") {
            playerSprite->setCurrAnimation("double-jumping");
            playerSprite->resetCurrAnimation();
        }
        playerSprite->enableLoop();
    }

    else if (player_data->curr_state == PlayerData::State::fall) {
        if (playerSprite->getCurrentAnimation() != "falling") {
            playerSprite->setCurrAnimation("falling");
            playerSprite->resetCurrAnimation();
        }
        playerSprite->enableLoop();
    }

    else if (player_data->curr_state == PlayerData::State::land) {
        playerSprite->setCurrAnimation("landing");
        playerSprite->enableLoop();
        float t = playerSprite->getCurrentAnimationTimeReached() / playerSprite->getCurrentAnimationTimeDuration();

        if (t >= 1.0f)
            player_data->curr_state = PlayerData::State::idle;
    }else if (glm::length(dir) != 0) {
        playerSprite->setCurrAnimation("run");
    }
    else {
        playerSprite->setCurrAnimation("idle");
    }



    //set player look direction
    GameObject* cam_go = scene.get_child("camera");
    float camera_angle{0.f};
    if (cam_go) {
        camera_angle = cam_go->getComponent<CameraData>(DataType::CAMERA)->angle;
    }
    const double pi = 3.14159265358979323846;
    float angle_under_two_pi = static_cast<float>(static_cast<int>((camera_angle * 180 / pi)) % 360) * (pi / 180);

    glm::vec3 n = glm::normalize(glm::vec3(sin(camera_angle + pi/2),0.f, cos(camera_angle + pi/2)));
    if (glm::dot(dir, n) < -std::numeric_limits<float>::epsilon() && isLookingLeft)
    {
        playerSprite->flipX();
        isLookingLeft = false;
    }
    else if (glm::dot(dir, n) > std::numeric_limits<float>::epsilon() && !isLookingLeft) {
        playerSprite->flipX();
        isLookingLeft = true;
    }

    playerSprite->setExternalTransform(position, temp->scale, temp->rotation);

    ///////////////////////////////
    // Render planting Animation //
    ///////////////////////////////

    /*
    * TODO: show this if and only I consider a planting animation necessary
    * 
    static bool show_plant_animation = false;

    if (player_data->curr_state == PlayerData::State::plant) {
        show_plant_animation = true;
        if (plantSprite->getCurrAnimation() != "plant") {
            plantSprite->setCurrAnimation("plant");
            plantSprite->resetCurrAnimation();
        }
    }

    if (show_plant_animation) {

        plantSprite->update(dt);

        glm::vec3 n = glm::normalize(glm::vec3(sin(camera_angle + pi / 2), 0.f, cos(camera_angle + pi / 2)));
        if (playerSprite->isFlipX())
            n = -n;
        n *= 10.0f;
        renderer.add(&*plantSprite, glm::vec3(position.x + n.x, position.y + n.y, position.z + n.z), glm::vec2(temp->scale.x, temp->scale.y), 0);
    }
    */
    /////////////////////////////
    // Render attack animation //
    /////////////////////////////
    /*
    static bool show_attack_animation = false;

    if (player_data->curr_state == PlayerData::State::attack) {
        slashSprite->setCurrAnimation("slash");
        show_attack_animation = true;
    }

    if (show_attack_animation) {

        slashSprite->update(dt);

        float t = slashSprite->getCurrTimeReached().asSeconds() / slashSprite->getCurrTimeDuration().asSeconds();
        if (t >= 1.0f) {
            show_attack_animation = false;
            player_data->curr_state = PlayerData::State::idle;
            slashSprite->resetCurrAnimation();
        }

        glm::vec3 n = glm::normalize(glm::vec3(sin(camera_angle + pi / 2), 0.f, cos(camera_angle + pi / 2)));
        if (playerSprite->isFlipX())
            n = -n;
        n *= 10.0f;
        renderer.add(&*slashSprite, glm::vec3(position.x + n.x, position.y + n.y, position.z + n.z), glm::vec2(temp->scale.x, temp->scale.y), 0);
    }
    */

    
}
